# Fabric Resources

The Fabric API (`/api/fabric`) manages a subset of SignalWire resource types in your project. Each resource surface below is exactly what this SDK exposes; non-listed resources either aren't exposed (deliberately — e.g. `swml_webhooks` and `cxml_webhooks` are not a creation-surface here, see below) or require a newer SDK release.

## Standard CRUD Pattern

All exposed resource types share the same methods:

```cpp
// List all resources of this type
auto items = client.fabric().agents.list();
items = client.fabric().agents.list({{"page", "2"}, {"page_size", "10"}});

// Create a new resource
auto agent = client.fabric().agents.create({
    {"name", "Support Bot"},
    {"prompt", {{"text", "You are a helpful support agent."}}},
});

// Get a resource by ID
auto got = client.fabric().agents.get("agent-uuid");

// Update a resource
client.fabric().agents.update("agent-uuid", {{"name", "Updated Name"}});

// Delete a resource
client.fabric().agents.del("agent-uuid");
```

Every `client.fabric().<name>` member is a `CrudResource` with `list / create / get / update / del`. The per-resource notes below cover only the exceptions.

## Resource Types

| Accessor                           | API Path                                    | Notes |
|------------------------------------|---------------------------------------------|-------|
| `fabric().subscribers`             | `/api/fabric/subscribers`                   | |
| `fabric().addresses`               | `/api/fabric/addresses`                     | |
| `fabric().sip_endpoints`           | `/api/fabric/sip_endpoints`                 | |
| `fabric().call_flows`              | `/api/fabric/call_flows`                    | |
| `fabric().swml_scripts`            | `/api/fabric/swml_scripts`                  | |
| `fabric().conferences`             | `/api/fabric/conferences`                   | |
| `fabric().resources`               | `/api/fabric/resources`                     | Generic-resource CRUD. |
| `fabric().tokens`                  | `/api/fabric/tokens`                        | |
| `fabric().routing`                 | `/api/fabric/routing`                       | |
| `fabric().agents`                  | `/api/fabric/agents`                        | AI Agent resources. |
| `fabric().domains`                 | `/api/fabric/domains`                       | |
| `fabric().topics`                  | `/api/fabric/topics`                        | |
| `fabric().webhooks`                | `/api/fabric/webhooks`                      | |

### Auto-materialized resource types (not exposed as a create surface)

Two resource types — `swml_webhook` and `cxml_webhook` — are **auto-materialized** by the server as a side-effect of binding a phone number to a call handler. They are deliberately **not exposed as a creation surface** on this SDK's Fabric namespace; creating them directly produces orphan resources that don't bind to any phone number.

To produce an `swml_webhook` or `cxml_webhook` resource, bind a phone number to the matching handler:

```cpp
// Auto-creates an swml_webhook Fabric resource
client.phone_numbers().set_swml_webhook(pn_id, "https://example.com/swml");

// Auto-creates a cxml_webhook Fabric resource
client.phone_numbers().set_cxml_webhook(pn_id, "https://example.com/voice.xml");
```

Full model and all 11 call-handler variants: see **[phone-binding.md](phone-binding.md)**.

## Binding a phone number to a handler

See **[phone-binding.md](phone-binding.md)** for the `PhoneCallHandler` enum, the mapping from each handler value to its auto-materialized Fabric resource, and the typed `phone_numbers().set_*` helpers. The one-liner summary:

```cpp
// SWML webhook (your backend returns SWML per call)
client.phone_numbers().set_swml_webhook(pn_id, "https://example.com/swml");
```

### `assign_phone_route` is not shipped

Some SDKs expose a `client.fabric().resources.assign_phone_route(...)` call that posts to `/api/fabric/resources/{id}/phone_routes`. That method does **not** bind a phone number to an SWML/cXML webhook or AI agent — those bindings live on the phone number (see [phone-binding.md](phone-binding.md)) and the Fabric resource is materialized automatically. `assign_phone_route` applies only to a narrow set of legacy resource types.

This SDK deliberately omits `assign_phone_route`. If you need it for a legacy resource type, drop to HTTP via `client.http_client().post(...)`.

## Generic Resources

Operate on any resource type by ID:

```cpp
// List all resources across all types
auto all_resources = client.fabric().resources.list();

// Get any resource by ID
auto resource = client.fabric().resources.get("resource-uuid");

// Delete any resource
client.fabric().resources.del("resource-uuid");
```
