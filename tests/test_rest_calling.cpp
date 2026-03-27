// REST Calling namespace tests
#include "signalwire/rest/rest_client.hpp"
using namespace signalwire::rest;
using json = nlohmann::json;

TEST(rest_calling_namespace_accessible) {
    RestClient client("example.signalwire.com", "proj", "tok");
    auto& calling = client.calling();
    (void)calling;
    return true;
}

TEST(rest_calling_has_client_ref) {
    RestClient client("example.signalwire.com", "proj", "tok");
    auto& calling = client.calling();
    // The calling namespace has a reference to the underlying HTTP client
    ASSERT_EQ(calling.client.base_url(), "https://example.signalwire.com");
    return true;
}

TEST(rest_phone_numbers_accessible) {
    RestClient client("example.signalwire.com", "proj", "tok");
    auto& pn = client.phone_numbers();
    (void)pn;
    return true;
}

TEST(rest_queues_accessible) {
    RestClient client("example.signalwire.com", "proj", "tok");
    auto& q = client.queues();
    (void)q;
    return true;
}

TEST(rest_recordings_accessible) {
    RestClient client("example.signalwire.com", "proj", "tok");
    auto& r = client.recordings();
    (void)r;
    return true;
}
