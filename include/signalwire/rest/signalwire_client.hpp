// Copyright (c) 2025 SignalWire
// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "signalwire/rest/http_client.hpp"

namespace signalwire {
namespace rest {

using json = nlohmann::json;

/// Top-level SignalWire REST client with all API namespaces
class SignalWireClient {
public:
    SignalWireClient(const std::string& space,
                     const std::string& project_id,
                     const std::string& token);

    /// Initialize from environment variables
    static SignalWireClient from_env();

    // ========================================================================
    // API Namespaces (all 21)
    // ========================================================================

    struct FabricNamespace {
        CrudResource subscribers;
        CrudResource addresses;
        CrudResource sip_endpoints;
        CrudResource call_flows;
        CrudResource swml_scripts;
        CrudResource conferences;
        CrudResource resources;
        CrudResource tokens;
        CrudResource routing;
        CrudResource agents;
        CrudResource domains;
        CrudResource topics;
        CrudResource webhooks;

        FabricNamespace(const HttpClient& c)
            : subscribers(c, "/api/fabric/subscribers"),
              addresses(c, "/api/fabric/addresses"),
              sip_endpoints(c, "/api/fabric/sip_endpoints"),
              call_flows(c, "/api/fabric/call_flows"),
              swml_scripts(c, "/api/fabric/swml_scripts"),
              conferences(c, "/api/fabric/conferences"),
              resources(c, "/api/fabric/resources"),
              tokens(c, "/api/fabric/tokens"),
              routing(c, "/api/fabric/routing"),
              agents(c, "/api/fabric/agents"),
              domains(c, "/api/fabric/domains"),
              topics(c, "/api/fabric/topics"),
              webhooks(c, "/api/fabric/webhooks") {}
    };

    struct CallingNamespace {
        const HttpClient& client;

        CallingNamespace(const HttpClient& c) : client(c) {}

        json dial(const json& params) const { return client.post("/api/calling/calls", params); }
        json list_calls(const std::map<std::string, std::string>& p = {}) const { return client.get("/api/calling/calls", p); }
        json get_call(const std::string& id) const { return client.get("/api/calling/calls/" + id); }
        json update_call(const std::string& id, const json& data) const { return client.put("/api/calling/calls/" + id, data); }
        json end_call(const std::string& id) const { return client.del("/api/calling/calls/" + id); }
        json play(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/play", data); }
        json record(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/record", data); }
        json collect(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/collect", data); }
        json tap(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/tap", data); }
        json detect(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/detect", data); }
        json connect(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/connect", data); }
        json send_digits(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/send_digits", data); }
        json transfer(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/transfer", data); }
        json answer(const std::string& id) const { return client.post("/api/calling/calls/" + id + "/answer"); }
        json hangup(const std::string& id) const { return client.post("/api/calling/calls/" + id + "/hangup"); }
        json hold(const std::string& id) const { return client.post("/api/calling/calls/" + id + "/hold"); }
        json unhold(const std::string& id) const { return client.post("/api/calling/calls/" + id + "/unhold"); }
    };

    struct PhoneNumbersNamespace : public CrudResource {
        PhoneNumbersNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/phone_numbers") {}
        json search(const std::map<std::string, std::string>& p) const { return client_.get(base_path_ + "/search", p); }
        json buy(const json& data) const { return client_.post(base_path_ + "/buy", data); }
        json release(const std::string& id) const { return client_.del(base_path_ + "/" + id); }
    };

    struct DatasphereNamespace {
        CrudResource documents;
        const HttpClient& client;

        DatasphereNamespace(const HttpClient& c)
            : documents(c, "/api/datasphere/documents"), client(c) {}

        json search(const json& data) const {
            return client.post("/api/datasphere/documents/search", data);
        }
    };

    struct VideoNamespace {
        CrudResource rooms;
        CrudResource room_sessions;
        CrudResource recordings;

        VideoNamespace(const HttpClient& c)
            : rooms(c, "/api/video/rooms"),
              room_sessions(c, "/api/video/room_sessions"),
              recordings(c, "/api/video/recordings") {}
    };

    struct CompatNamespace {
        const HttpClient& client;

        CompatNamespace(const HttpClient& c) : client(c) {}

        json create_call(const json& data) const { return client.post("/api/laml/2010-04-01/Accounts/calls", data); }
        json send_message(const json& data) const { return client.post("/api/laml/2010-04-01/Accounts/messages", data); }
        json list_calls(const std::map<std::string, std::string>& p = {}) const { return client.get("/api/laml/2010-04-01/Accounts/calls", p); }
        json list_messages(const std::map<std::string, std::string>& p = {}) const { return client.get("/api/laml/2010-04-01/Accounts/messages", p); }
    };

    // Simple CRUD namespaces
    struct AddressesNamespace : public CrudResource {
        AddressesNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/addresses") {}
    };

    struct QueuesNamespace : public CrudResource {
        QueuesNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/queues") {}
    };

    struct RecordingsNamespace : public CrudResource {
        RecordingsNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/recordings") {}
    };

    struct NumberGroupsNamespace : public CrudResource {
        NumberGroupsNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/number_groups") {}
    };

    struct VerifiedCallersNamespace : public CrudResource {
        VerifiedCallersNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/verified_callers") {}
    };

    struct SipProfileNamespace : public CrudResource {
        SipProfileNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/sip_profile") {}
    };

    struct LookupNamespace {
        const HttpClient& client;
        LookupNamespace(const HttpClient& c) : client(c) {}
        json lookup(const std::string& number) const { return client.get("/api/relay/rest/lookup/" + number); }
    };

    struct ShortCodesNamespace : public CrudResource {
        ShortCodesNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/short_codes") {}
    };

    struct ImportedNumbersNamespace : public CrudResource {
        ImportedNumbersNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/imported_numbers") {}
    };

    struct MFANamespace {
        const HttpClient& client;
        MFANamespace(const HttpClient& c) : client(c) {}
        json request_code(const json& data) const { return client.post("/api/mfa/request", data); }
        json verify_code(const json& data) const { return client.post("/api/mfa/verify", data); }
    };

    struct RegistryNamespace : public CrudResource {
        RegistryNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/registry") {}
    };

    struct LogsNamespace : public CrudResource {
        LogsNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/logs") {}
    };

    struct ProjectNamespace {
        const HttpClient& client;
        ProjectNamespace(const HttpClient& c) : client(c) {}
        json get_project() const { return client.get("/api/relay/rest/project"); }
        json update_project(const json& data) const { return client.put("/api/relay/rest/project", data); }
    };

    struct PubSubNamespace {
        const HttpClient& client;
        PubSubNamespace(const HttpClient& c) : client(c) {}
        json publish(const json& data) const { return client.post("/api/pubsub/publish", data); }
    };

    struct ChatNamespace {
        const HttpClient& client;
        ChatNamespace(const HttpClient& c) : client(c) {}
        json send_message(const json& data) const { return client.post("/api/chat/messages", data); }
        json list_messages(const std::map<std::string, std::string>& p = {}) const { return client.get("/api/chat/messages", p); }
    };

    // ========================================================================
    // Namespace accessors
    // ========================================================================

    FabricNamespace& fabric() { return *fabric_; }
    CallingNamespace& calling() { return *calling_; }
    PhoneNumbersNamespace& phone_numbers() { return *phone_numbers_; }
    DatasphereNamespace& datasphere() { return *datasphere_; }
    VideoNamespace& video() { return *video_; }
    CompatNamespace& compat() { return *compat_; }
    AddressesNamespace& addresses() { return *addresses_; }
    QueuesNamespace& queues() { return *queues_; }
    RecordingsNamespace& recordings() { return *recordings_; }
    NumberGroupsNamespace& number_groups() { return *number_groups_; }
    VerifiedCallersNamespace& verified_callers() { return *verified_callers_; }
    SipProfileNamespace& sip_profile() { return *sip_profile_; }
    LookupNamespace& lookup() { return *lookup_; }
    ShortCodesNamespace& short_codes() { return *short_codes_; }
    ImportedNumbersNamespace& imported_numbers() { return *imported_numbers_; }
    MFANamespace& mfa() { return *mfa_; }
    RegistryNamespace& registry() { return *registry_; }
    LogsNamespace& logs() { return *logs_; }
    ProjectNamespace& project() { return *project_; }
    PubSubNamespace& pubsub() { return *pubsub_; }
    ChatNamespace& chat() { return *chat_; }

    /// Get the underlying HTTP client
    const HttpClient& http_client() const { return *client_; }

private:
    std::unique_ptr<HttpClient> client_;

    std::unique_ptr<FabricNamespace> fabric_;
    std::unique_ptr<CallingNamespace> calling_;
    std::unique_ptr<PhoneNumbersNamespace> phone_numbers_;
    std::unique_ptr<DatasphereNamespace> datasphere_;
    std::unique_ptr<VideoNamespace> video_;
    std::unique_ptr<CompatNamespace> compat_;
    std::unique_ptr<AddressesNamespace> addresses_;
    std::unique_ptr<QueuesNamespace> queues_;
    std::unique_ptr<RecordingsNamespace> recordings_;
    std::unique_ptr<NumberGroupsNamespace> number_groups_;
    std::unique_ptr<VerifiedCallersNamespace> verified_callers_;
    std::unique_ptr<SipProfileNamespace> sip_profile_;
    std::unique_ptr<LookupNamespace> lookup_;
    std::unique_ptr<ShortCodesNamespace> short_codes_;
    std::unique_ptr<ImportedNumbersNamespace> imported_numbers_;
    std::unique_ptr<MFANamespace> mfa_;
    std::unique_ptr<RegistryNamespace> registry_;
    std::unique_ptr<LogsNamespace> logs_;
    std::unique_ptr<ProjectNamespace> project_;
    std::unique_ptr<PubSubNamespace> pubsub_;
    std::unique_ptr<ChatNamespace> chat_;
};

} // namespace rest
} // namespace signalwire
