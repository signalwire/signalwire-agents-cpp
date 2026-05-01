// Copyright (c) 2025 SignalWire
// SPDX-License-Identifier: MIT
//
// mocktest.hpp -- C++ test helper for the porting-sdk mock_signalwire HTTP
// server. Mirrors the Python conftest fixtures (signalwire_client + mock) and
// the Go pilot pkg/rest/internal/mocktest so unit tests can exercise the real
// SDK code path against a real HTTP server backed by SignalWire's 13 OpenAPI
// specs.
//
// The mock server is per-process: the first call to ensure_server() probes
// http://127.0.0.1:<port>/__mock__/health and either confirms a running
// instance or spawns one as a subprocess. Each test gets a freshly reset
// journal/scenario state via reset() before it runs.
//
// The default port is 8772 (matching the C++ rollout slot). Override via the
// MOCK_SIGNALWIRE_PORT environment variable to share a pre-running instance.
#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "signalwire/rest/rest_client.hpp"

namespace signalwire {
namespace rest {
namespace mocktest {

using nlohmann::json;

// JournalEntry mirrors mock_signalwire.journal.JournalEntry over the wire.
struct JournalEntry {
    double timestamp = 0.0;
    std::string method;
    std::string path;
    std::map<std::string, std::vector<std::string>> query_params;
    std::map<std::string, std::string> headers;
    json body;                   // Either a JSON object/array or a string
    std::optional<std::string> matched_route;
    std::optional<int> response_status;

    // Convenience: returns true iff the request body parses as a JSON object.
    bool body_is_object() const { return body.is_object(); }
};

// Probe-or-spawn the mock server. Returns the configured base URL once the
// server answers /__mock__/health. Throws std::runtime_error if startup
// fails. Idempotent across calls within the test process.
std::string ensure_server();

// Resolve the port the helper will target. Honours MOCK_SIGNALWIRE_PORT;
// otherwise falls back to 8772 (the C++ rollout slot).
int resolve_port();

// Reset the mock server's journal + scenarios. Tests call this at the top of
// each test (or via the make_client factory which does it for them).
void journal_reset();

// Read every journal entry recorded since the last reset, in arrival order.
std::vector<JournalEntry> journal();

// Return the most recent journal entry. Throws std::runtime_error when the
// journal is empty -- every test that hits the mock must produce at least
// one entry, so an empty journal is always a test bug.
JournalEntry journal_last();

// Stage a one-shot response override for the named operation id. Subsequent
// hits fall back to spec synthesis. The endpoint id is the OpenAPI
// operationId; the active list is exposed at /__mock__/scenarios.
void scenario_set(const std::string& endpoint_id,
                  int status,
                  const json& body);

// Build a RestClient pointed at the mock server, using project="test_proj"
// and token="test_tok" -- matches the Python signalwire_client fixture and
// the Go mocktest.New entry point. Resets journal/scenarios before
// returning so each test starts clean.
RestClient make_client();

} // namespace mocktest
} // namespace rest
} // namespace signalwire
