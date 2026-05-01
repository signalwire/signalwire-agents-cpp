// Copyright (c) 2025 SignalWire
// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <memory>
#include <optional>
#include <nlohmann/json.hpp>
#include "signalwire/rest/http_client.hpp"
#include "signalwire/rest/phone_call_handler.hpp"

namespace signalwire {
namespace rest {

using json = nlohmann::json;

/// Top-level SignalWire REST client with all API namespaces
class RestClient {
public:
    RestClient(const std::string& space,
                     const std::string& project_id,
                     const std::string& token);

    /// Initialize from environment variables
    static RestClient from_env();

    /// Construct with an explicit pre-built base URL (`http://...` or
    /// `https://...`) instead of synthesizing one from the SignalWire
    /// space hostname. Used by audit harnesses pointing the client at
    /// loopback fixtures. The space-based constructor remains the
    /// production path.
    static RestClient with_base_url(const std::string& base_url,
                                     const std::string& project_id,
                                     const std::string& token);

    /// Project ID accessor (read-only).
    const std::string& project_id() const { return project_id_; }

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

        // ----------------------------------------------------------------
        // Command-dispatch entry point (matches Python CallingNamespace).
        //
        // All 32 commands are POST /api/calling/calls with a body shaped
        // {"command": "<name>", "id": "<call_id>"?, "params": {...}}.
        // ``id`` is omitted for calls that target no specific call (e.g.
        // ``dial`` and ``update``).
        // ----------------------------------------------------------------
        json execute(const std::string& command,
                     const json& params,
                     const std::optional<std::string>& call_id = std::nullopt) const {
            json body = {{"command", command}, {"params", params}};
            if (call_id) body["id"] = *call_id;
            return client.post("/api/calling/calls", body);
        }

        // Lifecycle
        json dial(const json& params) const { return execute("dial", params); }
        json update(const json& params) const { return execute("update", params); }
        json end(const std::string& call_id, const json& params = json::object()) const {
            return execute("calling.end", params, call_id);
        }
        json transfer(const std::string& call_id, const json& params) const {
            return execute("calling.transfer", params, call_id);
        }
        json disconnect(const std::string& call_id, const json& params = json::object()) const {
            return execute("calling.disconnect", params, call_id);
        }

        // Play
        json play(const std::string& call_id, const json& params) const {
            return execute("calling.play", params, call_id);
        }
        json play_pause(const std::string& call_id, const json& params) const {
            return execute("calling.play.pause", params, call_id);
        }
        json play_resume(const std::string& call_id, const json& params) const {
            return execute("calling.play.resume", params, call_id);
        }
        json play_stop(const std::string& call_id, const json& params) const {
            return execute("calling.play.stop", params, call_id);
        }
        json play_volume(const std::string& call_id, const json& params) const {
            return execute("calling.play.volume", params, call_id);
        }

        // Record
        json record(const std::string& call_id, const json& params) const {
            return execute("calling.record", params, call_id);
        }
        json record_pause(const std::string& call_id, const json& params) const {
            return execute("calling.record.pause", params, call_id);
        }
        json record_resume(const std::string& call_id, const json& params) const {
            return execute("calling.record.resume", params, call_id);
        }
        json record_stop(const std::string& call_id, const json& params) const {
            return execute("calling.record.stop", params, call_id);
        }

        // Collect
        json collect(const std::string& call_id, const json& params) const {
            return execute("calling.collect", params, call_id);
        }
        json collect_stop(const std::string& call_id, const json& params) const {
            return execute("calling.collect.stop", params, call_id);
        }
        json collect_start_input_timers(const std::string& call_id,
                                        const json& params) const {
            return execute("calling.collect.start_input_timers", params, call_id);
        }

        // Detect
        json detect(const std::string& call_id, const json& params) const {
            return execute("calling.detect", params, call_id);
        }
        json detect_stop(const std::string& call_id, const json& params) const {
            return execute("calling.detect.stop", params, call_id);
        }

        // Tap
        json tap(const std::string& call_id, const json& params) const {
            return execute("calling.tap", params, call_id);
        }
        json tap_stop(const std::string& call_id, const json& params) const {
            return execute("calling.tap.stop", params, call_id);
        }

        // Stream
        json stream(const std::string& call_id, const json& params) const {
            return execute("calling.stream", params, call_id);
        }
        json stream_stop(const std::string& call_id, const json& params) const {
            return execute("calling.stream.stop", params, call_id);
        }

        // Denoise
        json denoise(const std::string& call_id,
                     const json& params = json::object()) const {
            return execute("calling.denoise", params, call_id);
        }
        json denoise_stop(const std::string& call_id, const json& params) const {
            return execute("calling.denoise.stop", params, call_id);
        }

        // Transcribe
        json transcribe(const std::string& call_id, const json& params) const {
            return execute("calling.transcribe", params, call_id);
        }
        json transcribe_stop(const std::string& call_id, const json& params) const {
            return execute("calling.transcribe.stop", params, call_id);
        }

        // AI
        json ai_message(const std::string& call_id, const json& params) const {
            return execute("calling.ai_message", params, call_id);
        }
        json ai_hold(const std::string& call_id,
                     const json& params = json::object()) const {
            return execute("calling.ai_hold", params, call_id);
        }
        json ai_unhold(const std::string& call_id,
                       const json& params = json::object()) const {
            return execute("calling.ai_unhold", params, call_id);
        }
        json ai_stop(const std::string& call_id,
                     const json& params = json::object()) const {
            return execute("calling.ai.stop", params, call_id);
        }

        // Live transcribe / translate
        json live_transcribe(const std::string& call_id, const json& params) const {
            return execute("calling.live_transcribe", params, call_id);
        }
        json live_translate(const std::string& call_id, const json& params) const {
            return execute("calling.live_translate", params, call_id);
        }

        // Fax
        json send_fax_stop(const std::string& call_id,
                           const json& params = json::object()) const {
            return execute("calling.send_fax.stop", params, call_id);
        }
        json receive_fax_stop(const std::string& call_id,
                              const json& params = json::object()) const {
            return execute("calling.receive_fax.stop", params, call_id);
        }

        // SIP refer
        json refer(const std::string& call_id, const json& params) const {
            return execute("calling.refer", params, call_id);
        }

        // Custom user event
        json user_event(const std::string& call_id, const json& params) const {
            return execute("calling.user_event", params, call_id);
        }

        // ----------------------------------------------------------------
        // Legacy convenience accessors (kept for backwards compatibility).
        // These use direct REST paths rather than the command-dispatch
        // wire format and are retained so existing tests keep linking.
        // ----------------------------------------------------------------
        json list_calls(const std::map<std::string, std::string>& p = {}) const { return client.get("/api/calling/calls", p); }
        json get_call(const std::string& id) const { return client.get("/api/calling/calls/" + id); }
        json update_call(const std::string& id, const json& data) const { return client.put("/api/calling/calls/" + id, data); }
        json end_call(const std::string& id) const { return client.del("/api/calling/calls/" + id); }
        json connect(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/connect", data); }
        json send_digits(const std::string& id, const json& data) const { return client.post("/api/calling/calls/" + id + "/send_digits", data); }
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

        // ====================================================================
        // Typed binding helpers
        //
        // Each ``set_*`` helper wraps ``update`` with the right ``call_handler``
        // value and companion field. The server auto-materializes the matching
        // Fabric resource; you do NOT need to pre-create ``swml_webhooks``,
        // ``cxml_webhooks``, or an AI agent and you do NOT need to call
        // ``assign_phone_route``. See ``rest/docs/phone-binding.md``.
        //
        // The ``make_*_body`` static helpers expose the JSON body that the
        // helper would send — useful for tests or when you need to amend the
        // body before calling ``update`` directly.
        // ====================================================================

        /// Options for binding a phone number to a cXML (Twilio-compat) webhook.
        struct CxmlWebhookOptions {
            std::optional<std::string> fallback_url;
            std::optional<std::string> status_callback_url;
        };

        /// Options for binding a phone number to a call flow.
        struct CallFlowOptions {
            /// Accepts ``"working_copy"`` or ``"current_deployed"``
            /// (server default when omitted).
            std::optional<std::string> version;
        };

        /// Options for binding a phone number to a RELAY topic.
        struct RelayTopicOptions {
            std::optional<std::string> status_callback_url;
        };

        // -- Body builders (pure, no side effects — useful for tests) --------

        static json make_swml_webhook_body(const std::string& url) {
            return {
                {"call_handler", to_wire_string(PhoneCallHandler::RelayScript)},
                {"call_relay_script_url", url},
            };
        }

        static json make_cxml_webhook_body(const std::string& url,
                                           const CxmlWebhookOptions& opts = {}) {
            json body = {
                {"call_handler", to_wire_string(PhoneCallHandler::LamlWebhooks)},
                {"call_request_url", url},
            };
            if (opts.fallback_url) body["call_fallback_url"] = *opts.fallback_url;
            if (opts.status_callback_url) body["call_status_callback_url"] = *opts.status_callback_url;
            return body;
        }

        static json make_cxml_application_body(const std::string& application_id) {
            return {
                {"call_handler", to_wire_string(PhoneCallHandler::LamlApplication)},
                {"call_laml_application_id", application_id},
            };
        }

        static json make_ai_agent_body(const std::string& agent_id) {
            return {
                {"call_handler", to_wire_string(PhoneCallHandler::AiAgent)},
                {"call_ai_agent_id", agent_id},
            };
        }

        static json make_call_flow_body(const std::string& flow_id,
                                        const CallFlowOptions& opts = {}) {
            json body = {
                {"call_handler", to_wire_string(PhoneCallHandler::CallFlow)},
                {"call_flow_id", flow_id},
            };
            if (opts.version) body["call_flow_version"] = *opts.version;
            return body;
        }

        static json make_relay_application_body(const std::string& name) {
            return {
                {"call_handler", to_wire_string(PhoneCallHandler::RelayApplication)},
                {"call_relay_application", name},
            };
        }

        static json make_relay_topic_body(const std::string& topic,
                                          const RelayTopicOptions& opts = {}) {
            json body = {
                {"call_handler", to_wire_string(PhoneCallHandler::RelayTopic)},
                {"call_relay_topic", topic},
            };
            if (opts.status_callback_url) body["call_relay_topic_status_callback_url"] = *opts.status_callback_url;
            return body;
        }

        // -- Typed helpers ---------------------------------------------------

        /// Route inbound calls to an SWML webhook URL.
        /// Server auto-creates a ``swml_webhook`` Fabric resource keyed off
        /// this URL.
        json set_swml_webhook(const std::string& resource_id, const std::string& url) const {
            return update(resource_id, make_swml_webhook_body(url));
        }

        /// Route inbound calls to a cXML (Twilio-compat / LAML) webhook.
        /// Despite the wire value ``laml_webhooks`` being plural, this creates
        /// a single ``cxml_webhook`` Fabric resource. Extra options populate
        /// fallback and status-callback fields.
        json set_cxml_webhook(const std::string& resource_id,
                              const std::string& url,
                              const CxmlWebhookOptions& opts = {}) const {
            return update(resource_id, make_cxml_webhook_body(url, opts));
        }

        /// Route inbound calls to an existing cXML application by ID.
        json set_cxml_application(const std::string& resource_id,
                                  const std::string& application_id) const {
            return update(resource_id, make_cxml_application_body(application_id));
        }

        /// Route inbound calls to an AI Agent Fabric resource by ID.
        json set_ai_agent(const std::string& resource_id,
                          const std::string& agent_id) const {
            return update(resource_id, make_ai_agent_body(agent_id));
        }

        /// Route inbound calls to a Call Flow by ID.
        /// ``opts.version`` accepts ``"working_copy"`` or ``"current_deployed"``.
        json set_call_flow(const std::string& resource_id,
                           const std::string& flow_id,
                           const CallFlowOptions& opts = {}) const {
            return update(resource_id, make_call_flow_body(flow_id, opts));
        }

        /// Route inbound calls to a named RELAY application.
        json set_relay_application(const std::string& resource_id,
                                   const std::string& name) const {
            return update(resource_id, make_relay_application_body(name));
        }

        /// Route inbound calls to a RELAY topic (client subscription).
        json set_relay_topic(const std::string& resource_id,
                             const std::string& topic,
                             const RelayTopicOptions& opts = {}) const {
            return update(resource_id, make_relay_topic_body(topic, opts));
        }
    };

    // Datasphere: documents are a CRUD resource with chunk sub-operations.
    // We keep the existing CrudResource-typed ``documents`` field for any
    // callers using it directly; add a richer DatasphereDocuments wrapper
    // with the search + chunk methods on a parallel ``documents_ext``-style
    // accessor not needed because the CrudResource methods (list/get/etc.)
    // already cover the basics. The chunk methods live directly on the
    // namespace via ``DatasphereDocumentsExt``.
    struct DatasphereDocuments : public CrudResource {
        DatasphereDocuments(const HttpClient& c)
            : CrudResource(c, "/api/datasphere/documents") {}
        json search(const json& data) const {
            return client_.post(base_path_ + "/search", data);
        }
        json list_chunks(const std::string& document_id,
                         const std::map<std::string, std::string>& params = {}) const {
            return client_.get(base_path_ + "/" + document_id + "/chunks", params);
        }
        json get_chunk(const std::string& document_id,
                       const std::string& chunk_id) const {
            return client_.get(base_path_ + "/" + document_id + "/chunks/" + chunk_id);
        }
        json delete_chunk(const std::string& document_id,
                          const std::string& chunk_id) const {
            return client_.del(base_path_ + "/" + document_id + "/chunks/" + chunk_id);
        }
    };

    struct DatasphereNamespace {
        DatasphereDocuments documents;
        const HttpClient& client;

        DatasphereNamespace(const HttpClient& c)
            : documents(c), client(c) {}

        // Legacy alias retained -- callers can still invoke
        // ``client.datasphere().search({...})`` instead of going through
        // ``documents.search``.
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

    // ---------------------------------------------------------------------
    // Compat sub-resources (Twilio-compatible LAML API). These mirror the
    // Python ``CompatCalls`` / ``CompatMessages`` / ``CompatFaxes`` /
    // ``CompatPhoneNumbers`` classes. All paths are scoped by the project
    // (account_sid) at the LAML root ``/api/laml/2010-04-01/Accounts/{sid}``.
    // ---------------------------------------------------------------------
    struct CompatCalls : public CrudResource {
        CompatCalls(const HttpClient& c, const std::string& base)
            : CrudResource(c, base) {}

        // Override update -- compat uses POST not PUT for resource updates.
        json update(const std::string& sid, const json& data) const {
            return client_.post(base_path_ + "/" + sid, data);
        }

        // Recording sub-resources.
        json start_recording(const std::string& call_sid, const json& data) const {
            return client_.post(base_path_ + "/" + call_sid + "/Recordings", data);
        }
        json update_recording(const std::string& call_sid,
                              const std::string& recording_sid,
                              const json& data) const {
            return client_.post(base_path_ + "/" + call_sid + "/Recordings/" + recording_sid, data);
        }

        // Stream sub-resources.
        json start_stream(const std::string& call_sid, const json& data) const {
            return client_.post(base_path_ + "/" + call_sid + "/Streams", data);
        }
        json stop_stream(const std::string& call_sid,
                         const std::string& stream_sid,
                         const json& data) const {
            return client_.post(base_path_ + "/" + call_sid + "/Streams/" + stream_sid, data);
        }
    };

    struct CompatMessages : public CrudResource {
        CompatMessages(const HttpClient& c, const std::string& base)
            : CrudResource(c, base) {}

        json update(const std::string& sid, const json& data) const {
            return client_.post(base_path_ + "/" + sid, data);
        }

        json list_media(const std::string& message_sid,
                        const std::map<std::string, std::string>& params = {}) const {
            return client_.get(base_path_ + "/" + message_sid + "/Media", params);
        }
        json get_media(const std::string& message_sid,
                       const std::string& media_sid) const {
            return client_.get(base_path_ + "/" + message_sid + "/Media/" + media_sid);
        }
        json delete_media(const std::string& message_sid,
                          const std::string& media_sid) const {
            return client_.del(base_path_ + "/" + message_sid + "/Media/" + media_sid);
        }
    };

    struct CompatFaxes : public CrudResource {
        CompatFaxes(const HttpClient& c, const std::string& base)
            : CrudResource(c, base) {}

        json update(const std::string& sid, const json& data) const {
            return client_.post(base_path_ + "/" + sid, data);
        }

        json list_media(const std::string& fax_sid,
                        const std::map<std::string, std::string>& params = {}) const {
            return client_.get(base_path_ + "/" + fax_sid + "/Media", params);
        }
        json get_media(const std::string& fax_sid,
                       const std::string& media_sid) const {
            return client_.get(base_path_ + "/" + fax_sid + "/Media/" + media_sid);
        }
        json delete_media(const std::string& fax_sid,
                          const std::string& media_sid) const {
            return client_.del(base_path_ + "/" + fax_sid + "/Media/" + media_sid);
        }
    };

    struct CompatPhoneNumbers {
        const HttpClient& client;
        std::string base_path;          // /api/laml/.../IncomingPhoneNumbers
        std::string available_base;     // /api/laml/.../AvailablePhoneNumbers
        std::string imported_base;      // /api/laml/.../ImportedPhoneNumbers

        CompatPhoneNumbers(const HttpClient& c, const std::string& account_base)
            : client(c),
              base_path(account_base + "/IncomingPhoneNumbers"),
              available_base(account_base + "/AvailablePhoneNumbers"),
              imported_base(account_base + "/ImportedPhoneNumbers") {}

        json list(const std::map<std::string, std::string>& params = {}) const {
            return client.get(base_path, params);
        }
        json get(const std::string& sid) const {
            return client.get(base_path + "/" + sid);
        }
        json update(const std::string& sid, const json& data) const {
            return client.post(base_path + "/" + sid, data);
        }
        json delete_(const std::string& sid) const {
            return client.del(base_path + "/" + sid);
        }
        json purchase(const json& data) const {
            return client.post(base_path, data);
        }
        json import_number(const json& data) const {
            return client.post(imported_base, data);
        }
        json list_available_countries(const std::map<std::string, std::string>& params = {}) const {
            return client.get(available_base, params);
        }
        json search_local(const std::string& country,
                          const std::map<std::string, std::string>& params = {}) const {
            return client.get(available_base + "/" + country + "/Local", params);
        }
        json search_toll_free(const std::string& country,
                              const std::map<std::string, std::string>& params = {}) const {
            return client.get(available_base + "/" + country + "/TollFree", params);
        }
    };

    struct CompatNamespace {
        const HttpClient& client;
        std::string account_base;

        // Sub-resources (Python parity).
        CompatCalls calls;
        CompatMessages messages;
        CompatFaxes faxes;
        CompatPhoneNumbers phone_numbers;

        CompatNamespace(const HttpClient& c, const std::string& account_sid)
            : client(c),
              account_base("/api/laml/2010-04-01/Accounts/" + account_sid),
              calls(c, account_base + "/Calls"),
              messages(c, account_base + "/Messages"),
              faxes(c, account_base + "/Faxes"),
              phone_numbers(c, account_base) {}

        // Legacy convenience methods retained for backwards compatibility
        // (the existing rest_compat_laml.cpp example references these).
        // The paths are intentionally not project-scoped to avoid changing
        // the wire contract any callers may rely on.
        json create_call(const json& data) const { return client.post("/api/laml/2010-04-01/Accounts/calls", data); }
        json send_message(const json& data) const { return client.post("/api/laml/2010-04-01/Accounts/messages", data); }
        json list_calls(const std::map<std::string, std::string>& p = {}) const { return client.get("/api/laml/2010-04-01/Accounts/calls", p); }
        json list_messages(const std::map<std::string, std::string>& p = {}) const { return client.get("/api/laml/2010-04-01/Accounts/messages", p); }
    };

    // ---------------------------------------------------------------------
    // Simple CRUD namespaces (Python parity additions). The existing CRUD
    // surface from CrudResource is preserved; we just augment with the
    // ``list / get / create / delete / update`` methods that Python exposes
    // with non-standard verbs (``addresses`` skips update; ``short_codes``
    // uses PUT; etc.) and add the sub-resource accessors required by the
    // mock-server tests.
    // ---------------------------------------------------------------------
    struct AddressesNamespace : public CrudResource {
        AddressesNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/addresses") {}
        // Python-parity ``delete`` alias (CrudResource exposes ``del``).
        json delete_(const std::string& id) const { return client_.del(base_path_ + "/" + id); }
    };

    struct QueuesNamespace : public CrudResource {
        QueuesNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/queues") {}
        // Override update -- Python uses PUT, not PATCH (the CrudResource default).
        json update(const std::string& id, const json& data) const {
            return client_.put(base_path_ + "/" + id, data);
        }
        json list_members(const std::string& queue_id,
                          const std::map<std::string, std::string>& params = {}) const {
            return client_.get(base_path_ + "/" + queue_id + "/members", params);
        }
        json get_next_member(const std::string& queue_id) const {
            return client_.get(base_path_ + "/" + queue_id + "/members/next");
        }
        json get_member(const std::string& queue_id, const std::string& member_id) const {
            return client_.get(base_path_ + "/" + queue_id + "/members/" + member_id);
        }
    };

    struct RecordingsNamespace : public CrudResource {
        RecordingsNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/recordings") {}
        // Python ``delete`` alias.
        json delete_(const std::string& id) const { return client_.del(base_path_ + "/" + id); }
    };

    struct NumberGroupsNamespace : public CrudResource {
        NumberGroupsNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/number_groups") {}
        // Python NumberGroups uses PUT for update (overrides default PATCH).
        json update(const std::string& id, const json& data) const {
            return client_.put(base_path_ + "/" + id, data);
        }
        // Membership operations.
        json list_memberships(const std::string& group_id,
                              const std::map<std::string, std::string>& params = {}) const {
            return client_.get(base_path_ + "/" + group_id + "/number_group_memberships", params);
        }
        json add_membership(const std::string& group_id, const json& data) const {
            return client_.post(base_path_ + "/" + group_id + "/number_group_memberships", data);
        }
        json get_membership(const std::string& membership_id) const {
            return client_.get("/api/relay/rest/number_group_memberships/" + membership_id);
        }
        json delete_membership(const std::string& membership_id) const {
            return client_.del("/api/relay/rest/number_group_memberships/" + membership_id);
        }
    };

    struct VerifiedCallersNamespace : public CrudResource {
        VerifiedCallersNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/verified_callers") {}
    };

    // SipProfile is a singleton (no resource id). We expose ``get`` /
    // ``update`` directly on the namespace (PUT for update -- Python parity).
    struct SipProfileNamespace : public CrudResource {
        SipProfileNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/sip_profile") {}
        json get() const { return client_.get(base_path_); }
        json update(const json& data) const { return client_.put(base_path_, data); }
    };

    struct LookupNamespace {
        const HttpClient& client;
        LookupNamespace(const HttpClient& c) : client(c) {}
        json lookup(const std::string& number) const { return client.get("/api/relay/rest/lookup/" + number); }
    };

    struct ShortCodesNamespace : public CrudResource {
        ShortCodesNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/short_codes") {}
        // Python ShortCodes.update uses PUT explicitly.
        json update(const std::string& id, const json& data) const {
            return client_.put(base_path_ + "/" + id, data);
        }
    };

    // The Python imported_numbers namespace lives at
    // /api/relay/rest/imported_phone_numbers and only exposes ``create``.
    // We retain the legacy /api/relay/rest/imported_numbers path for callers
    // who hit it via list/get/update/delete and add the Python-parity
    // ``create`` that posts to the imported_phone_numbers endpoint.
    struct ImportedNumbersNamespace : public CrudResource {
        ImportedNumbersNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/imported_numbers") {}
        // Python parity: POST /api/relay/rest/imported_phone_numbers.
        json create(const json& data) const {
            return client_.post("/api/relay/rest/imported_phone_numbers", data);
        }
    };

    struct MFANamespace {
        const HttpClient& client;
        MFANamespace(const HttpClient& c) : client(c) {}
        // Python parity:
        json sms(const json& data) const { return client.post("/api/relay/rest/mfa/sms", data); }
        json call(const json& data) const { return client.post("/api/relay/rest/mfa/call", data); }
        json verify(const std::string& request_id, const json& data) const {
            return client.post("/api/relay/rest/mfa/" + request_id + "/verify", data);
        }
        // Legacy convenience accessors.
        json request_code(const json& data) const { return client.post("/api/mfa/request", data); }
        json verify_code(const json& data) const { return client.post("/api/mfa/verify", data); }
    };

    struct RegistryNamespace : public CrudResource {
        RegistryNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/registry") {}
    };

    struct LogsNamespace : public CrudResource {
        LogsNamespace(const HttpClient& c) : CrudResource(c, "/api/relay/rest/logs") {}
    };

    // Project tokens (Python: client.project.tokens.{create,update,delete}).
    struct ProjectTokens {
        const HttpClient& client;
        const std::string base_path = "/api/project/tokens";
        ProjectTokens(const HttpClient& c) : client(c) {}
        json create(const json& data) const { return client.post(base_path, data); }
        json update(const std::string& token_id, const json& data) const {
            return client.patch(base_path + "/" + token_id, data);
        }
        json delete_(const std::string& token_id) const {
            return client.del(base_path + "/" + token_id);
        }
    };

    struct ProjectNamespace {
        const HttpClient& client;
        ProjectTokens tokens;
        ProjectNamespace(const HttpClient& c) : client(c), tokens(c) {}
        // Legacy direct accessors -- these don't have Python equivalents
        // and remain for backwards compatibility with existing tests.
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
    /// Helper used by both constructors to wire all 21 namespaces against
    /// the (already-built) HttpClient.
    void init_namespaces();

    std::string project_id_;

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
