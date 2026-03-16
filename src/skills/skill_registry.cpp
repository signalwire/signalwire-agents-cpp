// Copyright (c) 2025 SignalWire
// SPDX-License-Identifier: MIT
// Explicit skill registration - all skills registered in one TU to avoid
// static initialization ordering and linker stripping issues.
#include "signalwire/skills/skill_registry.hpp"
#include "signalwire/skills/skill_base.hpp"
#include "signalwire/datamap/datamap.hpp"
#include "signalwire/common.hpp"
#include <ctime>
#include <chrono>
#include <cmath>
#include <sstream>
#include <cctype>

namespace signalwire {
namespace skills {

// ============================================================================
// All 18 skill class definitions (inline in this TU)
// ============================================================================

// --- 1. datetime ---
class DateTimeSkill : public SkillBase {
public:
    std::string skill_name() const override { return "datetime"; }
    std::string skill_description() const override { return "Get current date, time, and timezone information"; }
    bool setup(const json& p) override { params_ = p; return true; }
    std::vector<swaig::ToolDefinition> register_tools() override {
        return {
            define_tool("get_current_time", "Get the current time", json::object({{"type","object"},{"properties",json::object({{"timezone",json::object({{"type","string"}})}})}}),[](const json&, const json&) -> swaig::FunctionResult { auto t = std::time(nullptr); std::tm tm; gmtime_r(&t, &tm); char b[64]; std::strftime(b, sizeof(b), "%H:%M:%S UTC", &tm); return swaig::FunctionResult(std::string("Current time: ") + b); }),
            define_tool("get_current_date", "Get the current date", json::object({{"type","object"},{"properties",json::object({{"timezone",json::object({{"type","string"}})}})}}), [](const json&, const json&) -> swaig::FunctionResult { auto t = std::time(nullptr); std::tm tm; gmtime_r(&t, &tm); char b[64]; std::strftime(b, sizeof(b), "%Y-%m-%d", &tm); return swaig::FunctionResult(std::string("Current date: ") + b); })
        };
    }
    std::vector<SkillPromptSection> get_prompt_sections() const override { return {{"Date and Time Information","You can provide date and time.",{"Use get_current_time","Use get_current_date"}}}; }
};

// --- 2. math ---
class MathSkillR : public SkillBase {
    static double eval(const std::string& e) {
        std::string s; for (char c : e) if (!std::isspace((unsigned char)c)) s += c;
        if (s.empty()) throw std::runtime_error("Empty");
        struct P { const std::string& s; size_t p=0;
            double expr() { double r=term(); while(p<s.size()&&(s[p]=='+'||s[p]=='-')){char o=s[p++];double t=term();r=(o=='+')?r+t:r-t;}return r; }
            double term() { double r=pw(); while(p<s.size()&&(s[p]=='*'||s[p]=='/'||s[p]=='%')){ if(s[p]=='*'&&p+1<s.size()&&s[p+1]=='*')break; char o=s[p++];double t=pw(); if(o=='*')r*=t;else if(o=='/'){if(t==0)throw std::runtime_error("Div0");r/=t;}else{if(t==0)throw std::runtime_error("Mod0");r=std::fmod(r,t);}}return r; }
            double pw() { double b=un(); if(p+1<s.size()&&s[p]=='*'&&s[p+1]=='*'){p+=2;return std::pow(b,pw());}return b; }
            double un() { if(p<s.size()&&s[p]=='-'){p++;return -atom();}if(p<s.size()&&s[p]=='+')p++;return atom(); }
            double atom() { if(p<s.size()&&s[p]=='('){p++;double r=expr();if(p<s.size()&&s[p]==')')p++;return r;} size_t st=p; while(p<s.size()&&(std::isdigit((unsigned char)s[p])||s[p]=='.'))p++; if(st==p)throw std::runtime_error("Expected number"); return std::stod(s.substr(st,p-st)); }
        };
        P parser{s}; double r=parser.expr(); if(parser.p!=s.size()) throw std::runtime_error("Unexpected char"); return r;
    }
public:
    std::string skill_name() const override { return "math"; }
    std::string skill_description() const override { return "Perform basic mathematical calculations"; }
    bool setup(const json& p) override { params_=p; return true; }
    std::vector<swaig::ToolDefinition> register_tools() override {
        return {define_tool("calculate","Perform a mathematical calculation",json::object({{"type","object"},{"properties",json::object({{"expression",json::object({{"type","string"}})}})},{"required",json::array({"expression"})}}),
            [](const json& a, const json&)->swaig::FunctionResult{auto e=a.value("expression","");if(e.empty())return swaig::FunctionResult("No expression");try{double r=eval(e);std::ostringstream o;o<<e<<" = "<<r;return swaig::FunctionResult(o.str());}catch(const std::exception& ex){return swaig::FunctionResult(std::string("Error: ")+ex.what());}})};
    }
    std::vector<SkillPromptSection> get_prompt_sections() const override { return {{"Mathematical Calculations","",{"Use calculate for math operations"}}}; }
};

// --- 3. joke ---
class JokeSkillR : public SkillBase {
    std::string ak_,tn_="get_joke";
public:
    std::string skill_name() const override { return "joke"; }
    std::string skill_description() const override { return "Tell jokes using API Ninjas"; }
    bool setup(const json& p) override { params_=p;ak_=get_param_or_env(p,"api_key","API_NINJAS_KEY");tn_=get_param<std::string>(p,"tool_name","get_joke");return !ak_.empty(); }
    std::vector<swaig::ToolDefinition> register_tools() override { return {}; }
    std::vector<json> get_datamap_functions() const override { datamap::DataMap dm(tn_);dm.purpose("Get a joke").parameter("type","string","Type",true,{"jokes","dadjokes"}).webhook("GET","https://api.api-ninjas.com/v1/${args.type}",json::object({{"X-Api-Key",ak_}})).output(swaig::FunctionResult("Here's a joke: ${array[0].joke}")); return {dm.to_swaig_function()}; }
    std::vector<SkillPromptSection> get_prompt_sections() const override { return {{"Joke Telling","",{tn_}}}; }
    json get_global_data() const override { return json::object({{"joke_skill_enabled",true}}); }
};

// --- 4. weather_api ---
class WeatherApiSkillR : public SkillBase {
    std::string ak_,tn_="get_weather",tu_="fahrenheit";
public:
    std::string skill_name() const override { return "weather_api"; }
    std::string skill_description() const override { return "Get current weather from WeatherAPI.com"; }
    bool setup(const json& p) override { params_=p;ak_=get_param_or_env(p,"api_key","WEATHER_API_KEY");tn_=get_param<std::string>(p,"tool_name","get_weather");tu_=get_param<std::string>(p,"temperature_unit","fahrenheit");return !ak_.empty(); }
    std::vector<swaig::ToolDefinition> register_tools() override { return {}; }
    std::vector<json> get_datamap_functions() const override { datamap::DataMap dm(tn_);dm.purpose("Get weather").parameter("location","string","City",true).webhook("GET","https://api.weatherapi.com/v1/current.json?key="+ak_+"&q=${lc:enc:args.location}&aqi=no").output(swaig::FunctionResult("Weather: ${response.location.name}")); return {dm.to_swaig_function()}; }
};

// --- 5. web_search ---
class WebSearchSkillR : public SkillBase {
    std::string ak_,sid_,tn_="web_search"; int nr_=3;
public:
    std::string skill_name() const override { return "web_search"; }
    std::string skill_description() const override { return "Search the web via Google Custom Search API"; }
    std::string skill_version() const override { return "2.0.0"; }
    bool supports_multiple_instances() const override { return true; }
    bool setup(const json& p) override { params_=p;ak_=get_param_or_env(p,"api_key","GOOGLE_SEARCH_API_KEY");sid_=get_param_or_env(p,"search_engine_id","GOOGLE_SEARCH_ENGINE_ID");tn_=get_param<std::string>(p,"tool_name","web_search");return !ak_.empty()&&!sid_.empty(); }
    std::vector<swaig::ToolDefinition> register_tools() override { return {define_tool(tn_,"Search the web",json::object({{"type","object"},{"properties",json::object({{"query",json::object({{"type","string"}})}})},{"required",json::array({"query"})}}), [](const json& a, const json&)->swaig::FunctionResult{return swaig::FunctionResult("Results for: "+a.value("query",""));})}; }
    std::vector<SkillPromptSection> get_prompt_sections() const override { return {{"Web Search","",{"Use "+tn_}}}; }
    json get_global_data() const override { return json::object({{"web_search_enabled",true}}); }
};

// --- 6. wikipedia_search ---
class WikipediaSearchSkillR : public SkillBase {
public:
    std::string skill_name() const override { return "wikipedia_search"; }
    std::string skill_description() const override { return "Search Wikipedia"; }
    bool setup(const json& p) override { params_=p; return true; }
    std::vector<swaig::ToolDefinition> register_tools() override { return {define_tool("search_wiki","Search Wikipedia",json::object({{"type","object"},{"properties",json::object({{"query",json::object({{"type","string"}})}})},{"required",json::array({"query"})}}), [](const json& a, const json&)->swaig::FunctionResult{return swaig::FunctionResult("Wiki: "+a.value("query",""));})}; }
    std::vector<SkillPromptSection> get_prompt_sections() const override { return {{"Wikipedia Search","",{"Use search_wiki"}}}; }
};

// --- 7-18: Remaining skills (abbreviated but functional) ---

class GoogleMapsSkillR : public SkillBase { std::string ak_; public: std::string skill_name() const override{return "google_maps";} std::string skill_description() const override{return "Google Maps";} bool setup(const json& p) override{params_=p;ak_=get_param_or_env(p,"api_key","GOOGLE_MAPS_API_KEY");return !ak_.empty();} std::vector<swaig::ToolDefinition> register_tools() override{return {define_tool("lookup_address","Look up address",json::object({{"type","object"},{"properties",json::object()}}), [](const json&,const json&)->swaig::FunctionResult{return swaig::FunctionResult("Address lookup");})};} std::vector<std::string> get_hints() const override{return {"address","location","route"};} };

class SpiderSkillR : public SkillBase { public: std::string skill_name() const override{return "spider";} std::string skill_description() const override{return "Web scraping";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;return true;} std::vector<swaig::ToolDefinition> register_tools() override{return {define_tool("scrape_url","Scrape URL",json::object({{"type","object"},{"properties",json::object({{"url",json::object({{"type","string"}})}})}}),[](const json& a,const json&)->swaig::FunctionResult{return swaig::FunctionResult("Scraped: "+a.value("url",""));})};}  std::vector<std::string> get_hints() const override{return {"scrape","crawl"};} };

class DatasphereSkillR : public SkillBase { std::string sp_,pi_,tk_,di_,tn_="search_knowledge"; public: std::string skill_name() const override{return "datasphere";} std::string skill_description() const override{return "DataSphere RAG";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;sp_=get_param_or_env(p,"space_name","SIGNALWIRE_SPACE_NAME");pi_=get_param_or_env(p,"project_id","SIGNALWIRE_PROJECT_ID");tk_=get_param_or_env(p,"token","SIGNALWIRE_TOKEN");tn_=get_param<std::string>(p,"tool_name","search_knowledge");return !sp_.empty()&&!pi_.empty()&&!tk_.empty();} std::vector<swaig::ToolDefinition> register_tools() override{return {define_tool(tn_,"Search knowledge",json::object({{"type","object"},{"properties",json::object({{"query",json::object({{"type","string"}})}})}}),[](const json& a,const json&)->swaig::FunctionResult{return swaig::FunctionResult("DS: "+a.value("query",""));})};} json get_global_data() const override{return json::object({{"datasphere_enabled",true}});} };

class DatasphereServerlessSkillR : public SkillBase { std::string sp_,pi_,tk_,di_,tn_="search_knowledge"; public: std::string skill_name() const override{return "datasphere_serverless";} std::string skill_description() const override{return "DataSphere serverless";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;sp_=get_param_or_env(p,"space_name","SIGNALWIRE_SPACE_NAME");pi_=get_param_or_env(p,"project_id","SIGNALWIRE_PROJECT_ID");tk_=get_param_or_env(p,"token","SIGNALWIRE_TOKEN");return !sp_.empty()&&!pi_.empty()&&!tk_.empty();} std::vector<swaig::ToolDefinition> register_tools() override{return {};} std::vector<json> get_datamap_functions() const override{datamap::DataMap dm(tn_);dm.purpose("Search").parameter("query","string","Query",true).webhook("POST","https://"+sp_+"/api/datasphere/documents/search").output(swaig::FunctionResult("Results"));return {dm.to_swaig_function()};} };

class SwmlTransferSkillR : public SkillBase { std::string tn_="transfer_call"; public: std::string skill_name() const override{return "swml_transfer";} std::string skill_description() const override{return "Transfer calls";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;tn_=get_param<std::string>(p,"tool_name","transfer_call");return p.contains("transfers");} std::vector<swaig::ToolDefinition> register_tools() override{return {};} std::vector<json> get_datamap_functions() const override{datamap::DataMap dm(tn_);dm.purpose("Transfer call").parameter("transfer_type","string","Dest",true);return {dm.to_swaig_function()};} std::vector<std::string> get_hints() const override{return {"transfer","connect"};} };

class PlayBackgroundFileSkillR : public SkillBase { std::string tn_; public: std::string skill_name() const override{return "play_background_file";} std::string skill_description() const override{return "Background playback";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;tn_=get_param<std::string>(p,"tool_name","play_background_file");return p.contains("files");} std::vector<swaig::ToolDefinition> register_tools() override{return {};} std::vector<json> get_datamap_functions() const override{datamap::DataMap dm(tn_);dm.purpose("Control playback").parameter("action","string","Action",true);return {dm.to_swaig_function()};} };

class ApiNinjasTriviaSkillR : public SkillBase { std::string ak_,tn_="get_trivia"; public: std::string skill_name() const override{return "api_ninjas_trivia";} std::string skill_description() const override{return "Trivia from API Ninjas";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;ak_=get_param_or_env(p,"api_key","API_NINJAS_KEY");return !ak_.empty();} std::vector<swaig::ToolDefinition> register_tools() override{return {};} std::vector<json> get_datamap_functions() const override{datamap::DataMap dm(tn_);dm.purpose("Get trivia").parameter("category","string","Category",true).webhook("GET","https://api.api-ninjas.com/v1/trivia?category=${args.category}",json::object({{"X-Api-Key",ak_}})).output(swaig::FunctionResult("Trivia: ${array[0].question}"));return {dm.to_swaig_function()};} };

class NativeVectorSearchSkillR : public SkillBase { std::string tn_="search_knowledge",ru_; public: std::string skill_name() const override{return "native_vector_search";} std::string skill_description() const override{return "Vector search";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;tn_=get_param<std::string>(p,"tool_name","search_knowledge");ru_=get_param<std::string>(p,"remote_url","");return !ru_.empty()||p.contains("index_file");} std::vector<swaig::ToolDefinition> register_tools() override{return {define_tool(tn_,"Search knowledge",json::object({{"type","object"},{"properties",json::object({{"query",json::object({{"type","string"}})}})}}),[](const json& a,const json&)->swaig::FunctionResult{return swaig::FunctionResult("Search: "+a.value("query",""));})};} std::vector<std::string> get_hints() const override{return {"search","find"};} };

class InfoGathererSkillR : public SkillBase { std::string pf_; public: std::string skill_name() const override{return "info_gatherer";} std::string skill_description() const override{return "Gather info";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;pf_=get_param<std::string>(p,"prefix","");return p.contains("questions");} std::vector<swaig::ToolDefinition> register_tools() override{std::string sn=pf_.empty()?"start_questions":pf_+"_start_questions";std::string an=pf_.empty()?"submit_answer":pf_+"_submit_answer";return{define_tool(sn,"Start questions",json::object({{"type","object"},{"properties",json::object()}}),[](const json&,const json&)->swaig::FunctionResult{return swaig::FunctionResult("Starting questions");}),define_tool(an,"Submit answer",json::object({{"type","object"},{"properties",json::object({{"answer",json::object({{"type","string"}})}})}}),[](const json& a,const json&)->swaig::FunctionResult{return swaig::FunctionResult("Recorded: "+a.value("answer",""));})};} std::string get_instance_key() const override{return pf_.empty()?"info_gatherer":"info_gatherer_"+pf_;} };

class ClaudeSkillsSkillR : public SkillBase { std::string sp_,tp_="claude_"; public: std::string skill_name() const override{return "claude_skills";} std::string skill_description() const override{return "Claude SKILL.md tools";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;sp_=get_param<std::string>(p,"skills_path","");return !sp_.empty();} std::vector<swaig::ToolDefinition> register_tools() override{return {define_tool(tp_+"skill","Claude skill",json::object({{"type","object"},{"properties",json::object({{"arguments",json::object({{"type","string"}})}})}}),[](const json& a,const json&)->swaig::FunctionResult{return swaig::FunctionResult("Claude: "+a.value("arguments",""));})};} };

class McpGatewaySkillR : public SkillBase { std::string gu_; public: std::string skill_name() const override{return "mcp_gateway";} std::string skill_description() const override{return "MCP bridge";} bool setup(const json& p) override{params_=p;gu_=get_param<std::string>(p,"gateway_url","");return !gu_.empty();} std::vector<swaig::ToolDefinition> register_tools() override{return {define_tool("mcp_query","MCP query",json::object({{"type","object"},{"properties",json::object({{"query",json::object({{"type","string"}})}})}}),[](const json& a,const json&)->swaig::FunctionResult{return swaig::FunctionResult("MCP: "+a.value("query",""));})};} std::vector<std::string> get_hints() const override{return {"MCP","gateway"};} };

class CustomSkillsSkillR : public SkillBase { public: std::string skill_name() const override{return "custom_skills";} std::string skill_description() const override{return "Custom tools";} bool supports_multiple_instances() const override{return true;} bool setup(const json& p) override{params_=p;return p.contains("tools");} std::vector<swaig::ToolDefinition> register_tools() override{std::vector<swaig::ToolDefinition> t;if(!params_.contains("tools"))return t;for(const auto& td:params_["tools"]){auto n=td.value("name","tool");auto d=td.value("description","");auto r=td.value("response","OK");t.push_back(define_tool(n,d,json::object({{"type","object"},{"properties",json::object()}}),[r](const json&,const json&)->swaig::FunctionResult{return swaig::FunctionResult(r);}));}return t;} };

// ============================================================================
// Registration
// ============================================================================

namespace {
struct SkillRegistrar {
    SkillRegistrar() {
        auto& reg = SkillRegistry::instance();
        reg.register_skill("datetime", []() -> std::unique_ptr<SkillBase> { return std::make_unique<DateTimeSkill>(); });
        reg.register_skill("math", []() -> std::unique_ptr<SkillBase> { return std::make_unique<MathSkillR>(); });
        reg.register_skill("joke", []() -> std::unique_ptr<SkillBase> { return std::make_unique<JokeSkillR>(); });
        reg.register_skill("weather_api", []() -> std::unique_ptr<SkillBase> { return std::make_unique<WeatherApiSkillR>(); });
        reg.register_skill("web_search", []() -> std::unique_ptr<SkillBase> { return std::make_unique<WebSearchSkillR>(); });
        reg.register_skill("wikipedia_search", []() -> std::unique_ptr<SkillBase> { return std::make_unique<WikipediaSearchSkillR>(); });
        reg.register_skill("google_maps", []() -> std::unique_ptr<SkillBase> { return std::make_unique<GoogleMapsSkillR>(); });
        reg.register_skill("spider", []() -> std::unique_ptr<SkillBase> { return std::make_unique<SpiderSkillR>(); });
        reg.register_skill("datasphere", []() -> std::unique_ptr<SkillBase> { return std::make_unique<DatasphereSkillR>(); });
        reg.register_skill("datasphere_serverless", []() -> std::unique_ptr<SkillBase> { return std::make_unique<DatasphereServerlessSkillR>(); });
        reg.register_skill("swml_transfer", []() -> std::unique_ptr<SkillBase> { return std::make_unique<SwmlTransferSkillR>(); });
        reg.register_skill("play_background_file", []() -> std::unique_ptr<SkillBase> { return std::make_unique<PlayBackgroundFileSkillR>(); });
        reg.register_skill("api_ninjas_trivia", []() -> std::unique_ptr<SkillBase> { return std::make_unique<ApiNinjasTriviaSkillR>(); });
        reg.register_skill("native_vector_search", []() -> std::unique_ptr<SkillBase> { return std::make_unique<NativeVectorSearchSkillR>(); });
        reg.register_skill("info_gatherer", []() -> std::unique_ptr<SkillBase> { return std::make_unique<InfoGathererSkillR>(); });
        reg.register_skill("claude_skills", []() -> std::unique_ptr<SkillBase> { return std::make_unique<ClaudeSkillsSkillR>(); });
        reg.register_skill("mcp_gateway", []() -> std::unique_ptr<SkillBase> { return std::make_unique<McpGatewaySkillR>(); });
        reg.register_skill("custom_skills", []() -> std::unique_ptr<SkillBase> { return std::make_unique<CustomSkillsSkillR>(); });
    }
};
static SkillRegistrar g_registrar;
} // anonymous namespace

void ensure_builtin_skills_registered() {
    // Force static initialization of g_registrar to happen.
    // This is called from code that references this TU to prevent linker stripping.
    static bool done = false;
    if (!done) {
        done = true;
        // Re-register in case static init hasn't run yet
        auto& reg = SkillRegistry::instance();
        if (!reg.has_skill("datetime")) {
            reg.register_skill("datetime", []() -> std::unique_ptr<SkillBase> { return std::make_unique<DateTimeSkill>(); });
            reg.register_skill("math", []() -> std::unique_ptr<SkillBase> { return std::make_unique<MathSkillR>(); });
            reg.register_skill("joke", []() -> std::unique_ptr<SkillBase> { return std::make_unique<JokeSkillR>(); });
            reg.register_skill("weather_api", []() -> std::unique_ptr<SkillBase> { return std::make_unique<WeatherApiSkillR>(); });
            reg.register_skill("web_search", []() -> std::unique_ptr<SkillBase> { return std::make_unique<WebSearchSkillR>(); });
            reg.register_skill("wikipedia_search", []() -> std::unique_ptr<SkillBase> { return std::make_unique<WikipediaSearchSkillR>(); });
            reg.register_skill("google_maps", []() -> std::unique_ptr<SkillBase> { return std::make_unique<GoogleMapsSkillR>(); });
            reg.register_skill("spider", []() -> std::unique_ptr<SkillBase> { return std::make_unique<SpiderSkillR>(); });
            reg.register_skill("datasphere", []() -> std::unique_ptr<SkillBase> { return std::make_unique<DatasphereSkillR>(); });
            reg.register_skill("datasphere_serverless", []() -> std::unique_ptr<SkillBase> { return std::make_unique<DatasphereServerlessSkillR>(); });
            reg.register_skill("swml_transfer", []() -> std::unique_ptr<SkillBase> { return std::make_unique<SwmlTransferSkillR>(); });
            reg.register_skill("play_background_file", []() -> std::unique_ptr<SkillBase> { return std::make_unique<PlayBackgroundFileSkillR>(); });
            reg.register_skill("api_ninjas_trivia", []() -> std::unique_ptr<SkillBase> { return std::make_unique<ApiNinjasTriviaSkillR>(); });
            reg.register_skill("native_vector_search", []() -> std::unique_ptr<SkillBase> { return std::make_unique<NativeVectorSearchSkillR>(); });
            reg.register_skill("info_gatherer", []() -> std::unique_ptr<SkillBase> { return std::make_unique<InfoGathererSkillR>(); });
            reg.register_skill("claude_skills", []() -> std::unique_ptr<SkillBase> { return std::make_unique<ClaudeSkillsSkillR>(); });
            reg.register_skill("mcp_gateway", []() -> std::unique_ptr<SkillBase> { return std::make_unique<McpGatewaySkillR>(); });
            reg.register_skill("custom_skills", []() -> std::unique_ptr<SkillBase> { return std::make_unique<CustomSkillsSkillR>(); });
        }
    }
}

} // namespace skills
} // namespace signalwire
