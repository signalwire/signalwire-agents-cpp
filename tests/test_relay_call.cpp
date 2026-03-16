// RELAY Call split tests — deeper call lifecycle, properties, action registry
#include "signalwire/relay/call.hpp"
#include "signalwire/relay/constants.hpp"
using namespace signalwire::relay;
using json = nlohmann::json;

TEST(relay_call_default_state_empty) {
    Call call;
    ASSERT_EQ(call.state(), "");
    ASSERT_EQ(call.direction(), "");
    ASSERT_EQ(call.from(), "");
    ASSERT_EQ(call.to(), "");
    ASSERT_EQ(call.tag(), "");
    return true;
}

TEST(relay_call_initial_state_is_created) {
    Call call("c-1", "n-1");
    ASSERT_EQ(call.state(), "created");
    return true;
}

TEST(relay_call_full_lifecycle) {
    Call call("c-2", "n-2");
    ASSERT_EQ(call.state(), "created");
    ASSERT_FALSE(call.is_answered());
    ASSERT_FALSE(call.is_ended());

    call.update_state("ringing");
    ASSERT_EQ(call.state(), "ringing");

    call.update_state("answered");
    ASSERT_TRUE(call.is_answered());

    call.update_state("ending");
    ASSERT_FALSE(call.is_answered());

    call.update_state("ended");
    ASSERT_TRUE(call.is_ended());
    return true;
}

TEST(relay_call_set_all_properties) {
    Call call("c-3", "n-3");
    call.set_direction("outbound");
    call.set_from("+15551111111");
    call.set_to("+15552222222");
    call.set_tag("tag-123");

    ASSERT_EQ(call.direction(), "outbound");
    ASSERT_EQ(call.from(), "+15551111111");
    ASSERT_EQ(call.to(), "+15552222222");
    ASSERT_EQ(call.tag(), "tag-123");
    return true;
}

TEST(relay_call_action_register_find_unregister) {
    Call call("c-4", "n-4");
    Action a1("ctl-1");
    Action a2("ctl-2");

    call.register_action("ctl-1", &a1);
    call.register_action("ctl-2", &a2);

    ASSERT_TRUE(call.find_action("ctl-1") != nullptr);
    ASSERT_TRUE(call.find_action("ctl-2") != nullptr);
    ASSERT_TRUE(call.find_action("ctl-3") == nullptr);

    call.unregister_action("ctl-1");
    ASSERT_TRUE(call.find_action("ctl-1") == nullptr);
    ASSERT_TRUE(call.find_action("ctl-2") != nullptr);
    return true;
}

TEST(relay_call_ended_resolves_all_actions) {
    Call call("c-5", "n-5");
    Action a1("ctl-10");
    Action a2("ctl-20");
    call.register_action("ctl-10", &a1);
    call.register_action("ctl-20", &a2);

    ASSERT_FALSE(a1.completed());
    ASSERT_FALSE(a2.completed());

    call.update_state("ended");

    ASSERT_TRUE(a1.completed());
    ASSERT_TRUE(a2.completed());
    return true;
}

TEST(relay_call_event_handler_receives_event) {
    Call call("c-6", "n-6");
    std::string captured_state;
    call.on_event([&](const CallEvent& ev) {
        captured_state = ev.call_state;
    });

    CallEvent ev;
    ev.event_type = "calling.call.state";
    ev.call_id = "c-6";
    ev.call_state = "answered";
    call.dispatch_event(ev);

    ASSERT_EQ(captured_state, "answered");
    return true;
}

TEST(relay_call_all_methods_return_completed_without_client) {
    Call call("c-7", "n-7");
    ASSERT_TRUE(call.answer().completed());
    ASSERT_TRUE(call.hangup().completed());
    ASSERT_TRUE(call.hold().completed());
    ASSERT_TRUE(call.unhold().completed());
    ASSERT_TRUE(call.disconnect().completed());
    return true;
}

TEST(relay_call_wait_for_ended_threaded) {
    Call call("c-8", "n-8");
    std::thread t([&call]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        call.update_state("ended");
    });
    bool ok = call.wait_for_ended(5000);
    ASSERT_TRUE(ok);
    t.join();
    return true;
}
