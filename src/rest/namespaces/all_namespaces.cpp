// Copyright (c) 2025 SignalWire
// SPDX-License-Identifier: MIT
// All REST API namespace implementations are inline in rest_client.hpp
// This file exists to satisfy the build system's expectation of source files
// in the namespaces directory.

// The namespace implementations are defined as nested structs within
// RestClient, which provides a clean API:
//   client.fabric().subscribers.list()
//   client.calling().dial({...})
//   client.phone_numbers().search({{"area_code", "512"}})
//   client.datasphere().search({{"query", "hello"}})
//   client.video().rooms.list()
//   client.compat().create_call({...})
//   client.addresses().list()
//   client.queues().list()
//   client.recordings().list()
//   client.number_groups().list()
//   client.verified_callers().list()
//   client.sip_profile().list()
//   client.lookup().lookup("+15551234567")
//   client.short_codes().list()
//   client.imported_numbers().list()
//   client.mfa().request_code({...})
//   client.registry().list()
//   client.logs().list()
//   client.project().get_project()
//   client.pubsub().publish({...})
//   client.chat().send_message({...})
