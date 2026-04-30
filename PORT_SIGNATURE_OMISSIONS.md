# PORT_SIGNATURE_OMISSIONS.md

Documented signature divergences between this C++ port and the Python
reference. Names-only divergences live in PORT_OMISSIONS.md /
PORT_ADDITIONS.md and are inherited automatically.

Excused divergences:

1. **Idiom-level**:
   - C++ constructors follow C++ conventions; param shapes differ from Python kwargs.
   - C++ methods return *this for fluent chaining; Python returns None.
   - C++ has no defaults expressible in Python (=NULL/=default initializer
     surface as required from the canonical view but optional in C++).
   - cv-qualifiers, references, smart pointers (shared_ptr/unique_ptr) all
     collapse to the underlying type in canonical form.

2. **Port maintenance backlog**: real param/arity divergences that should
   be reduced as the C++ port catches up to Python.


## Idiom: C++ constructors

signalwire.agent_server.AgentServer.__init__: C++ constructor signature follows C++ conventions
signalwire.core.agent_base.AgentBase.__init__: C++ constructor signature follows C++ conventions
signalwire.core.contexts.Context.__init__: C++ constructor signature follows C++ conventions
signalwire.core.contexts.ContextBuilder.__init__: C++ constructor signature follows C++ conventions
signalwire.core.contexts.GatherInfo.__init__: C++ constructor signature follows C++ conventions
signalwire.core.contexts.GatherQuestion.__init__: C++ constructor signature follows C++ conventions
signalwire.core.contexts.Step.__init__: C++ constructor signature follows C++ conventions
signalwire.core.function_result.FunctionResult.__init__: C++ constructor signature follows C++ conventions
signalwire.core.security.session_manager.SessionManager.__init__: C++ constructor signature follows C++ conventions
signalwire.core.skill_manager.SkillManager.__init__: C++ constructor signature follows C++ conventions
signalwire.core.swml_service.SWMLService.__init__: C++ constructor signature follows C++ conventions
signalwire.prefabs.concierge.ConciergeAgent.__init__: C++ constructor signature follows C++ conventions
signalwire.prefabs.faq_bot.FAQBotAgent.__init__: C++ constructor signature follows C++ conventions
signalwire.prefabs.info_gatherer.InfoGathererAgent.__init__: C++ constructor signature follows C++ conventions
signalwire.prefabs.receptionist.ReceptionistAgent.__init__: C++ constructor signature follows C++ conventions
signalwire.prefabs.survey.SurveyAgent.__init__: C++ constructor signature follows C++ conventions
signalwire.relay.call.Call.__init__: C++ constructor signature follows C++ conventions
signalwire.relay.client.RelayClient.__init__: C++ constructor signature follows C++ conventions
signalwire.relay.message.Message.__init__: C++ constructor signature follows C++ conventions
signalwire.rest._base.HttpClient.__init__: C++ constructor signature follows C++ conventions
signalwire.rest._base.SignalWireRestError.__init__: C++ constructor signature follows C++ conventions
signalwire.rest.client.RestClient.__init__: C++ constructor signature follows C++ conventions
signalwire.search.DocumentProcessor.__init__: C++ constructor signature follows C++ conventions
signalwire.search.IndexBuilder.__init__: C++ constructor signature follows C++ conventions
signalwire.search.SearchEngine.__init__: C++ constructor signature follows C++ conventions
signalwire.search.SearchService.__init__: C++ constructor signature follows C++ conventions
signalwire.search.search_service.SearchRequest.__init__: C++ constructor signature follows C++ conventions
signalwire.search.search_service.SearchResponse.__init__: C++ constructor signature follows C++ conventions
signalwire.search.search_service.SearchResult.__init__: C++ constructor signature follows C++ conventions
signalwire.swml.service.Service.__init__: C++ constructor signature follows C++ conventions

## Idiom: C++ fluent return *this

signalwire.core.contexts.Context.get_step: C++ fluent / builder pattern returns *this for chaining
signalwire.core.contexts.ContextBuilder.get_context: C++ fluent / builder pattern returns *this for chaining
signalwire.relay.call.Call.disconnect: C++ fluent / builder pattern returns *this for chaining
signalwire.relay.call.Call.hold: C++ fluent / builder pattern returns *this for chaining
signalwire.relay.call.Call.unhold: C++ fluent / builder pattern returns *this for chaining

## Backlog: real signature divergences (230 symbols)

signalwire.agent_server.AgentServer.run: BACKLOG / param-count-mismatch/ reference has 5 param(s), port has 1/ reference=['self', 'event', 'context', 'ho; return-mismatch/
signalwire.core.agent_base.AgentBase.add_answer_verb: BACKLOG / param-count-mismatch/ reference has 2 param(s), port has 3/ reference=['self', 'config'] port=['self',
signalwire.core.agent_base.AgentBase.add_post_ai_verb: BACKLOG / param-mismatch/ param[2] (config)/ name 'config' vs 'params'; type 'dict<string,any>' vs 'int'
signalwire.core.agent_base.AgentBase.add_post_answer_verb: BACKLOG / param-mismatch/ param[2] (config)/ name 'config' vs 'params'; type 'dict<string,any>' vs 'int'
signalwire.core.agent_base.AgentBase.add_pre_answer_verb: BACKLOG / param-mismatch/ param[2] (config)/ name 'config' vs 'params'; type 'dict<string,any>' vs 'int'
signalwire.core.agent_base.AgentBase.auto_map_sip_usernames: BACKLOG / param-count-mismatch/ reference has 1 param(s), port has 2/ reference=['self'] port=['self', 'enable']
signalwire.core.agent_base.AgentBase.enable_sip_routing: BACKLOG / param-count-mismatch/ reference has 3 param(s), port has 2/ reference=['self', 'auto_map', 'path'] por
signalwire.core.agent_base.AgentBase.on_debug_event: BACKLOG / param-mismatch/ param[1] (handler)/ name 'handler' vs 'cb'; type 'class/Callable' vs 'class/sign; return-mismatch/ retur
signalwire.core.agent_base.AgentBase.on_summary: BACKLOG / param-count-mismatch/ reference has 3 param(s), port has 2/ reference=['self', 'summary', 'raw_data'] ; return-mismatch/
signalwire.core.agent_base.AgentBase.operator=: BACKLOG / missing-reference/ in port, not in reference
signalwire.core.agent_base.AgentBase.register_sip_username: BACKLOG / param-mismatch/ param[1] (sip_username)/ name 'sip_username' vs 'username'
signalwire.core.contexts.Context.add_bullets: BACKLOG / param-mismatch/ param[2] (bullets)/ type 'list<string>' vs 'int'
signalwire.core.contexts.Context.add_enter_filler: BACKLOG / param-mismatch/ param[1] (language_code)/ name 'language_code' vs 'lang'; param-mismatch/ param[2] (fillers)/ type 'list
signalwire.core.contexts.Context.add_exit_filler: BACKLOG / param-mismatch/ param[1] (language_code)/ name 'language_code' vs 'lang'; param-mismatch/ param[2] (fillers)/ type 'list
signalwire.core.contexts.Context.add_step: BACKLOG / param-mismatch/ param[2] (task)/ kind 'keyword' vs 'positional'; type 'optional<string>' vs 'str; param-mismatch/ param[
signalwire.core.contexts.Context.add_system_bullets: BACKLOG / param-mismatch/ param[2] (bullets)/ type 'list<string>' vs 'int'
signalwire.core.contexts.Context.set_consolidate: BACKLOG / param-mismatch/ param[1] (consolidate)/ name 'consolidate' vs 'c'
signalwire.core.contexts.Context.set_enter_fillers: BACKLOG / param-mismatch/ param[1] (enter_fillers)/ name 'enter_fillers' vs 'fillers'; type 'dict<string,l
signalwire.core.contexts.Context.set_exit_fillers: BACKLOG / param-mismatch/ param[1] (exit_fillers)/ name 'exit_fillers' vs 'fillers'; type 'dict<string,lis
signalwire.core.contexts.Context.set_full_reset: BACKLOG / param-mismatch/ param[1] (full_reset)/ name 'full_reset' vs 'fr'
signalwire.core.contexts.Context.set_post_prompt: BACKLOG / param-mismatch/ param[1] (post_prompt)/ name 'post_prompt' vs 'pp'
signalwire.core.contexts.Context.set_system_prompt: BACKLOG / param-mismatch/ param[1] (system_prompt)/ name 'system_prompt' vs 'sp'
signalwire.core.contexts.Context.set_user_prompt: BACKLOG / param-mismatch/ param[1] (user_prompt)/ name 'user_prompt' vs 'up'
signalwire.core.contexts.Context.set_valid_contexts: BACKLOG / param-mismatch/ param[1] (contexts)/ name 'contexts' vs 'ctxs'; type 'list<string>' vs 'int'
signalwire.core.contexts.Context.set_valid_steps: BACKLOG / param-mismatch/ param[1] (steps)/ type 'list<string>' vs 'int'
signalwire.core.contexts.GatherInfo.add_question: BACKLOG / param-count-mismatch/ reference has 4 param(s), port has 7/ reference=['self', 'key', 'question', 'kwa
signalwire.core.contexts.Step.add_bullets: BACKLOG / param-mismatch/ param[2] (bullets)/ type 'list<string>' vs 'int'
signalwire.core.contexts.Step.add_gather_question: BACKLOG / param-mismatch/ param[3] (type)/ default 'string' vs None; param-mismatch/ param[4] (confirm)/ default False vs None
signalwire.core.contexts.Step.set_functions: BACKLOG / param-mismatch/ param[1] (functions)/ type 'union<list<string>,string>' vs 'int'
signalwire.core.contexts.Step.set_gather_info: BACKLOG / param-mismatch/ param[1] (output_key)/ type 'optional<string>' vs 'string'; param-mismatch/ param[2] (completion_action)
signalwire.core.contexts.Step.set_reset_consolidate: BACKLOG / param-mismatch/ param[1] (consolidate)/ name 'consolidate' vs 'c'
signalwire.core.contexts.Step.set_reset_full_reset: BACKLOG / param-mismatch/ param[1] (full_reset)/ name 'full_reset' vs 'fr'
signalwire.core.contexts.Step.set_reset_system_prompt: BACKLOG / param-mismatch/ param[1] (system_prompt)/ name 'system_prompt' vs 'sp'
signalwire.core.contexts.Step.set_reset_user_prompt: BACKLOG / param-mismatch/ param[1] (user_prompt)/ name 'user_prompt' vs 'up'
signalwire.core.contexts.Step.set_valid_contexts: BACKLOG / param-mismatch/ param[1] (contexts)/ name 'contexts' vs 'ctxs'; type 'list<string>' vs 'int'
signalwire.core.contexts.Step.set_valid_steps: BACKLOG / param-mismatch/ param[1] (steps)/ type 'list<string>' vs 'int'
signalwire.core.data_map.DataMap.body: BACKLOG / param-mismatch/ param[1] (data)/ type 'dict<string,any>' vs 'int'
signalwire.core.data_map.DataMap.description: BACKLOG / param-mismatch/ param[1] (description)/ name 'description' vs 'desc'
signalwire.core.data_map.DataMap.error_keys: BACKLOG / param-mismatch/ param[1] (keys)/ type 'list<string>' vs 'int'
signalwire.core.data_map.DataMap.expression: BACKLOG / param-mismatch/ param[2] (pattern)/ type 'union<class/Pattern,string>' vs 'string'; param-mismatch/ param[4] (nomatch_ou
signalwire.core.data_map.DataMap.foreach: BACKLOG / param-mismatch/ param[1] (foreach_config)/ type 'dict<string,any>' vs 'int'
signalwire.core.data_map.DataMap.global_error_keys: BACKLOG / param-mismatch/ param[1] (keys)/ type 'list<string>' vs 'int'
signalwire.core.data_map.DataMap.parameter: BACKLOG / param-mismatch/ param[3] (description)/ name 'description' vs 'desc'; param-mismatch/ param[4] (required)/ default False
signalwire.core.data_map.DataMap.params: BACKLOG / param-mismatch/ param[1] (data)/ type 'dict<string,any>' vs 'int'
signalwire.core.data_map.DataMap.purpose: BACKLOG / param-mismatch/ param[1] (description)/ name 'description' vs 'desc'
signalwire.core.data_map.DataMap.to_swaig_function: BACKLOG / return-mismatch/ returns 'dict<string,any>' vs 'int'
signalwire.core.data_map.DataMap.webhook: BACKLOG / param-mismatch/ param[3] (headers)/ type 'optional<dict<string,string>>' vs 'int'; param-mismatch/ param[4] (form_param)
signalwire.core.data_map.DataMap.webhook_expressions: BACKLOG / param-mismatch/ param[1] (expressions)/ type 'list<dict<string,any>>' vs 'int'
signalwire.core.function_result.FunctionResult.add_action: BACKLOG / param-mismatch/ param[2] (data)/ type 'any' vs 'int'
signalwire.core.function_result.FunctionResult.add_actions: BACKLOG / param-mismatch/ param[1] (actions)/ type 'list<dict<string,any>>' vs 'int'
signalwire.core.function_result.FunctionResult.add_dynamic_hints: BACKLOG / param-mismatch/ param[1] (hints)/ type 'list<union<dict<string,any>,string>>' vs 'int'
signalwire.core.function_result.FunctionResult.connect: BACKLOG / param-mismatch/ param[2] (final)/ default True vs None; param-mismatch/ param[3] (from_addr)/ type 'optional<string>' vs
signalwire.core.function_result.FunctionResult.create_payment_action: BACKLOG / return-mismatch/ returns 'dict<string,string>' vs 'int'
signalwire.core.function_result.FunctionResult.create_payment_parameter: BACKLOG / return-mismatch/ returns 'dict<string,string>' vs 'int'
signalwire.core.function_result.FunctionResult.create_payment_prompt: BACKLOG / param-mismatch/ param[1] (actions)/ type 'list<dict<string,string>>' vs 'int'; param-mismatch/ param[2] (card_type)/ typ
signalwire.core.function_result.FunctionResult.enable_extensive_data: BACKLOG / param-mismatch/ param[1] (enabled)/ default True vs None
signalwire.core.function_result.FunctionResult.enable_functions_on_timeout: BACKLOG / param-mismatch/ param[1] (enabled)/ default True vs None
signalwire.core.function_result.FunctionResult.execute_rpc: BACKLOG / param-mismatch/ param[2] (params)/ type 'optional<dict<string,any>>' vs 'int'; param-mismatch/ param[3] (call_id)/ type 
signalwire.core.function_result.FunctionResult.execute_swml: BACKLOG / param-mismatch/ param[1] (swml_content)/ type 'any' vs 'int'; param-mismatch/ param[2] (transfer)/ default False vs None
signalwire.core.function_result.FunctionResult.hold: BACKLOG / param-mismatch/ param[1] (timeout)/ default 300 vs None
signalwire.core.function_result.FunctionResult.join_conference: BACKLOG / param-count-mismatch/ reference has 19 param(s), port has 4/ reference=['self', 'name', 'muted', 'beep
signalwire.core.function_result.FunctionResult.pay: BACKLOG / param-mismatch/ param[2] (input_method)/ default 'dtmf' vs None; param-mismatch/ param[3] (status_url)/ type 'optional<s
signalwire.core.function_result.FunctionResult.play_background_file: BACKLOG / param-mismatch/ param[2] (wait)/ default False vs None
signalwire.core.function_result.FunctionResult.record_call: BACKLOG / param-mismatch/ param[1] (control_id)/ type 'optional<string>' vs 'string'; param-mismatch/ param[2] (stereo)/ default F
signalwire.core.function_result.FunctionResult.remove_global_data: BACKLOG / param-mismatch/ param[1] (keys)/ type 'union<list<string>,string>' vs 'int'
signalwire.core.function_result.FunctionResult.remove_metadata: BACKLOG / param-mismatch/ param[1] (keys)/ type 'union<list<string>,string>' vs 'int'
signalwire.core.function_result.FunctionResult.replace_in_history: BACKLOG / param-mismatch/ param[1] (text)/ type 'union<bool,string>' vs 'int'; required False vs True; def
signalwire.core.function_result.FunctionResult.rpc_ai_message: BACKLOG / param-mismatch/ param[3] (role)/ default 'system' vs None
signalwire.core.function_result.FunctionResult.rpc_dial: BACKLOG / param-mismatch/ param[4] (device_type)/ default 'phone' vs None
signalwire.core.function_result.FunctionResult.send_sms: BACKLOG / param-mismatch/ param[1] (to_number)/ name 'to_number' vs 'to'; param-mismatch/ param[2] (from_number)/ name 'from_numbe
signalwire.core.function_result.FunctionResult.set_metadata: BACKLOG / param-mismatch/ param[1] (data)/ type 'dict<string,any>' vs 'int'
signalwire.core.function_result.FunctionResult.set_post_process: BACKLOG / param-mismatch/ param[1] (post_process)/ name 'post_process' vs 'pp'
signalwire.core.function_result.FunctionResult.stop_record_call: BACKLOG / param-mismatch/ param[1] (control_id)/ type 'optional<string>' vs 'string'
signalwire.core.function_result.FunctionResult.stop_tap: BACKLOG / param-mismatch/ param[1] (control_id)/ type 'optional<string>' vs 'string'
signalwire.core.function_result.FunctionResult.switch_context: BACKLOG / param-mismatch/ param[1] (system_prompt)/ type 'optional<string>' vs 'string'; param-mismatch/ param[2] (user_prompt)/ t
signalwire.core.function_result.FunctionResult.swml_transfer: BACKLOG / param-mismatch/ param[3] (final)/ default True vs None
signalwire.core.function_result.FunctionResult.swml_user_event: BACKLOG / param-mismatch/ param[1] (event_data)/ type 'dict<string,any>' vs 'int'
signalwire.core.function_result.FunctionResult.tap: BACKLOG / param-mismatch/ param[2] (control_id)/ type 'optional<string>' vs 'string'; param-mismatch/ param[3] (direction)/ defaul
signalwire.core.function_result.FunctionResult.toggle_functions: BACKLOG / param-mismatch/ param[1] (function_toggles)/ type 'list<dict<string,any>>' vs 'int'
signalwire.core.function_result.FunctionResult.update_global_data: BACKLOG / param-mismatch/ param[1] (data)/ type 'dict<string,any>' vs 'int'
signalwire.core.function_result.FunctionResult.update_settings: BACKLOG / param-mismatch/ param[1] (settings)/ type 'dict<string,any>' vs 'int'
signalwire.core.function_result.FunctionResult.wait_for_user: BACKLOG / param-mismatch/ param[3] (answer_first)/ default False vs None
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.add_function_include: BACKLOG / param-count-mismatch/ reference has 4 param(s), port has 2/ reference=['self', 'url', 'functions', 'me
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.add_hints: BACKLOG / param-mismatch/ param[1] (hints)/ type 'list<string>' vs 'int'
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.add_internal_filler: BACKLOG / param-count-mismatch/ reference has 4 param(s), port has 3/ reference=['self', 'function_name', 'langu
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.add_language: BACKLOG / param-count-mismatch/ reference has 8 param(s), port has 2/ reference=['self', 'name', 'code', 'voice'
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.add_mcp_server: BACKLOG / param-mismatch/ param[2] (headers)/ type 'optional<dict<string,string>>' vs 'int'; param-mismatch/ param[3] (resources)/
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.add_pattern_hint: BACKLOG / param-count-mismatch/ reference has 5 param(s), port has 2/ reference=['self', 'hint', 'pattern', 'rep
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.add_pronunciation: BACKLOG / param-mismatch/ param[1] (replace)/ name 'replace' vs 'replace_val'; param-mismatch/ param[2] (with_text)/ name 'with_te
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.enable_debug_events: BACKLOG / param-mismatch/ param[1] (level)/ name 'level' vs 'enable'; type 'int' vs 'bool'; default 1 vs N
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.enable_mcp_server: BACKLOG / param-count-mismatch/ reference has 1 param(s), port has 2/ reference=['self'] port=['self', 'enable']
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_function_includes: BACKLOG / param-mismatch/ param[1] (includes)/ type 'list<dict<string,any>>' vs 'int'
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_global_data: BACKLOG / param-mismatch/ param[1] (data)/ type 'dict<string,any>' vs 'int'
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_internal_fillers: BACKLOG / param-mismatch/ param[1] (internal_fillers)/ name 'internal_fillers' vs 'fillers'; type 'dict<st
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_languages: BACKLOG / param-mismatch/ param[1] (languages)/ name 'languages' vs 'langs'; type 'list<dict<string,any>>'
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_native_functions: BACKLOG / param-mismatch/ param[1] (function_names)/ name 'function_names' vs 'funcs'; type 'list<string>'
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_param: BACKLOG / param-mismatch/ param[2] (value)/ type 'any' vs 'int'
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_params: BACKLOG / param-mismatch/ param[1] (params)/ type 'dict<string,any>' vs 'int'
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_post_prompt_llm_params: BACKLOG / param-mismatch/ param[1] (params)/ kind 'var_keyword' vs 'positional'; type 'any' vs 'int'; requ
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_prompt_llm_params: BACKLOG / param-mismatch/ param[1] (params)/ kind 'var_keyword' vs 'positional'; type 'any' vs 'int'; requ
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.set_pronunciations: BACKLOG / param-mismatch/ param[1] (pronunciations)/ name 'pronunciations' vs 'pronuns'; type 'list<dict<s
signalwire.core.mixins.ai_config_mixin.AIConfigMixin.update_global_data: BACKLOG / param-mismatch/ param[1] (data)/ type 'dict<string,any>' vs 'int'
signalwire.core.mixins.prompt_mixin.PromptMixin.define_contexts: BACKLOG / param-count-mismatch/ reference has 2 param(s), port has 1/ reference=['self', 'contexts'] port=['self; return-mismatch/
signalwire.core.mixins.prompt_mixin.PromptMixin.get_prompt: BACKLOG / return-mismatch/ returns 'union<list<dict<string,any>>,string>' vs 'string'
signalwire.core.mixins.prompt_mixin.PromptMixin.prompt_add_section: BACKLOG / param-count-mismatch/ reference has 7 param(s), port has 4/ reference=['self', 'title', 'body', 'bulle
signalwire.core.mixins.prompt_mixin.PromptMixin.prompt_add_subsection: BACKLOG / param-mismatch/ param[3] (body)/ default '' vs None; param-mismatch/ param[4] (bullets)/ type 'optional<list<string>>' v
signalwire.core.mixins.prompt_mixin.PromptMixin.prompt_add_to_section: BACKLOG / param-count-mismatch/ reference has 5 param(s), port has 4/ reference=['self', 'title', 'body', 'bulle
signalwire.core.mixins.skill_mixin.SkillMixin.add_skill: BACKLOG / param-mismatch/ param[2] (params)/ type 'optional<dict<string,any>>' vs 'int'
signalwire.core.mixins.skill_mixin.SkillMixin.list_skills: BACKLOG / return-mismatch/ returns 'list<string>' vs 'int'
signalwire.core.mixins.tool_mixin.ToolMixin.define_tool: BACKLOG / param-count-mismatch/ reference has 11 param(s), port has 2/ reference=['self', 'name', 'description',
signalwire.core.mixins.tool_mixin.ToolMixin.on_function_call: BACKLOG / param-mismatch/ param[2] (args)/ type 'dict<string,any>' vs 'int'; param-mismatch/ param[3] (raw_data)/ type 'optional<d
signalwire.core.mixins.tool_mixin.ToolMixin.register_swaig_function: BACKLOG / param-mismatch/ param[1] (function_dict)/ name 'function_dict' vs 'func_def'; type 'dict<string,
signalwire.core.mixins.web_mixin.WebMixin.enable_debug_routes: BACKLOG / param-count-mismatch/ reference has 1 param(s), port has 2/ reference=['self'] port=['self', 'enable']
signalwire.core.mixins.web_mixin.WebMixin.manual_set_proxy_url: BACKLOG / param-mismatch/ param[1] (proxy_url)/ name 'proxy_url' vs 'url'
signalwire.core.mixins.web_mixin.WebMixin.run: BACKLOG / param-count-mismatch/ reference has 6 param(s), port has 1/ reference=['self', 'event', 'context', 'fo; return-mismatch/
signalwire.core.mixins.web_mixin.WebMixin.serve: BACKLOG / param-count-mismatch/ reference has 3 param(s), port has 1/ reference=['self', 'host', 'port'] port=['
signalwire.core.mixins.web_mixin.WebMixin.set_dynamic_config_callback: BACKLOG / param-mismatch/ param[1] (callback)/ name 'callback' vs 'cb'; type 'callable<list<dict<any,any>,
signalwire.core.security.session_manager.SessionManager.validate_token: BACKLOG / param-mismatch/ param[1] (call_id)/ name 'call_id' vs 'token'; param-mismatch/ param[3] (token)/ name 'token' vs 'call_i
signalwire.core.skill_base.SkillBase.define_tool: BACKLOG / param-count-mismatch/ reference has 2 param(s), port has 6/ reference=['self', 'kwargs'] port=['self',; return-mismatch/
signalwire.core.skill_base.SkillBase.get_global_data: BACKLOG / return-mismatch/ returns 'dict<string,any>' vs 'int'
signalwire.core.skill_base.SkillBase.get_hints: BACKLOG / return-mismatch/ returns 'list<string>' vs 'int'
signalwire.core.skill_base.SkillBase.get_parameter_schema: BACKLOG / param-mismatch/ param[0] (cls)/ name 'cls' vs 'self'; kind 'cls' vs 'self'; return-mismatch/ returns 'dict<string,dict<s
signalwire.core.skill_base.SkillBase.get_prompt_sections: BACKLOG / return-mismatch/ returns 'list<dict<string,any>>' vs 'int'
signalwire.core.skill_base.SkillBase.register_tools: BACKLOG / return-mismatch/ returns 'void' vs 'int'
signalwire.core.skill_base.SkillBase.setup: BACKLOG / param-count-mismatch/ reference has 1 param(s), port has 2/ reference=['self'] port=['self', 'params']
signalwire.core.skill_manager.SkillManager.load_skill: BACKLOG / param-mismatch/ param[2] (skill_class)/ name 'skill_class' vs 'params'; type 'class/signalwire.c; param-mismatch/ param[
signalwire.core.skill_manager.SkillManager.unload_skill: BACKLOG / param-mismatch/ param[1] (skill_identifier)/ name 'skill_identifier' vs 'skill_name'; return-mismatch/ returns 'bool' vs
signalwire.core.swml_service.SWMLService.add_verb: BACKLOG / missing-port/ in reference, not in port
signalwire.core.swml_service.SWMLService.serve: BACKLOG / missing-port/ in reference, not in port
signalwire.core.swml_service.SWMLService.stop: BACKLOG / missing-port/ in reference, not in port
signalwire.relay.call.Call.ai: BACKLOG / param-count-mismatch/ reference has 16 param(s), port has 2/ reference=['self', 'control_id', 'agent',; return-mismatch/
signalwire.relay.call.Call.answer: BACKLOG / param-count-mismatch/ reference has 2 param(s), port has 1/ reference=['self', 'kwargs'] port=['self']; return-mismatch/
signalwire.relay.call.Call.collect: BACKLOG / param-count-mismatch/ reference has 11 param(s), port has 2/ reference=['self', 'digits', 'speech', 'i; return-mismatch/
signalwire.relay.call.Call.connect: BACKLOG / param-count-mismatch/ reference has 8 param(s), port has 2/ reference=['self', 'devices', 'ringback', ; return-mismatch/
signalwire.relay.call.Call.detect: BACKLOG / param-count-mismatch/ reference has 6 param(s), port has 2/ reference=['self', 'detect', 'timeout', 'c; return-mismatch/
signalwire.relay.call.Call.hangup: BACKLOG / param-mismatch/ param[1] (reason)/ default 'hangup' vs None; return-mismatch/ returns 'dict<any,any>' vs 'class/signalwi
signalwire.relay.call.Call.join_conference: BACKLOG / param-count-mismatch/ reference has 22 param(s), port has 3/ reference=['self', 'name', 'muted', 'beep; return-mismatch/
signalwire.relay.call.Call.join_room: BACKLOG / param-count-mismatch/ reference has 4 param(s), port has 2/ reference=['self', 'name', 'status_url', '; return-mismatch/
signalwire.relay.call.Call.live_transcribe: BACKLOG / param-count-mismatch/ reference has 3 param(s), port has 2/ reference=['self', 'action', 'kwargs'] por; return-mismatch/
signalwire.relay.call.Call.live_translate: BACKLOG / param-count-mismatch/ reference has 4 param(s), port has 2/ reference=['self', 'action', 'status_url',; return-mismatch/
signalwire.relay.call.Call.play: BACKLOG / param-count-mismatch/ reference has 8 param(s), port has 3/ reference=['self', 'media', 'volume', 'dir; return-mismatch/
signalwire.relay.call.Call.receive_fax: BACKLOG / param-count-mismatch/ reference has 4 param(s), port has 1/ reference=['self', 'control_id', 'on_compl; return-mismatch/
signalwire.relay.call.Call.record: BACKLOG / param-count-mismatch/ reference has 5 param(s), port has 2/ reference=['self', 'audio', 'control_id', ; return-mismatch/
signalwire.relay.call.Call.send_digits: BACKLOG / param-count-mismatch/ reference has 3 param(s), port has 2/ reference=['self', 'digits', 'control_id']; return-mismatch/
signalwire.relay.call.Call.send_fax: BACKLOG / param-count-mismatch/ reference has 7 param(s), port has 3/ reference=['self', 'document', 'identity',; return-mismatch/
signalwire.relay.call.Call.transfer: BACKLOG / param-count-mismatch/ reference has 3 param(s), port has 2/ reference=['self', 'dest', 'kwargs'] port=; return-mismatch/
signalwire.relay.call.Call.wait_for_ended: BACKLOG / param-mismatch/ param[1] (timeout)/ name 'timeout' vs 'timeout_ms'; type 'optional<float>' vs 'i; return-mismatch/ retur
signalwire.relay.client.RelayClient.connect: BACKLOG / return-mismatch/ returns 'void' vs 'bool'
signalwire.relay.client.RelayClient.dial: BACKLOG / param-count-mismatch/ reference has 5 param(s), port has 2/ reference=['self', 'devices', 'tag', 'max_
signalwire.relay.client.RelayClient.execute: BACKLOG / param-mismatch/ param[2] (params)/ type 'dict<string,any>' vs 'int'; return-mismatch/ returns 'dict<any,any>' vs 'int'
signalwire.relay.client.RelayClient.on_call: BACKLOG / param-mismatch/ param[1] (handler)/ type 'class/signalwire.relay.client.CallHandler' vs 'class/s; return-mismatch/ retur
signalwire.relay.client.RelayClient.on_message: BACKLOG / param-mismatch/ param[1] (handler)/ type 'class/signalwire.relay.client.MessageHandler' vs 'clas; return-mismatch/ retur
signalwire.relay.client.RelayClient.operator=: BACKLOG / missing-reference/ in port, not in reference
signalwire.relay.client.RelayClient.relay_protocol: BACKLOG / missing-reference/ in port, not in reference
signalwire.relay.client.RelayClient.send_message: BACKLOG / param-count-mismatch/ reference has 9 param(s), port has 7/ reference=['self', 'to_number', 'from_numb; return-mismatch/
signalwire.relay.message.Message.wait: BACKLOG / param-mismatch/ param[1] (timeout)/ name 'timeout' vs 'timeout_ms'; type 'optional<float>' vs 'i; return-mismatch/ retur
signalwire.rest._base.CrudResource.create: BACKLOG / missing-reference/ in port, not in reference
signalwire.rest._base.CrudResource.get: BACKLOG / missing-reference/ in port, not in reference
signalwire.rest._base.CrudResource.list: BACKLOG / missing-reference/ in port, not in reference
signalwire.rest._base.CrudResource.update: BACKLOG / missing-reference/ in port, not in reference
signalwire.rest._base.HttpClient.get: BACKLOG / missing-reference/ in port, not in reference
signalwire.rest._base.HttpClient.patch: BACKLOG / missing-reference/ in port, not in reference
signalwire.rest._base.HttpClient.post: BACKLOG / missing-reference/ in port, not in reference
signalwire.rest._base.HttpClient.put: BACKLOG / missing-reference/ in port, not in reference
signalwire.search.preprocess_document_content: BACKLOG / missing-port/ in reference, not in port
signalwire.search.preprocess_query: BACKLOG / missing-port/ in reference, not in port
signalwire.skills.registry.SkillRegistry.list_skills: BACKLOG / return-mismatch/ returns 'list<dict<string,string>>' vs 'int'
signalwire.skills.registry.SkillRegistry.register_skill: BACKLOG / param-count-mismatch/ reference has 2 param(s), port has 3/ reference=['self', 'skill_class'] port=['s
signalwire.swml.service.Service.add_verb: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.ai: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.amazon_bedrock: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.answer: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.auth_password: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.auth_username: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.build_tool_registry_json: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.cond: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.connect: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.define_tool: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.denoise: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.detect_machine: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.document: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.enter_queue: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.execute: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.extract_introspect_payload: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.generate_random_hex: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.goto_section: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.hangup: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.has_tool: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.join_conference: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.join_room: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.label: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.list_tool_names: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.live_transcribe: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.live_translate: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.name: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.on_function_call: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.pay: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.play: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.port: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.prompt: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.receive_fax: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.record: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.record_call: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.register_swaig_function: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.render_swml: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.request: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.return_section: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.route: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.send_digits: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.send_fax: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.send_sms: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.serve: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.set: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.set_auth: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.set_host: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.set_name: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.set_port: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.set_route: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.sip_refer: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.sleep: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.stop: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.stop_denoise: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.stop_record_call: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.stop_tap: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.switch_section: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.tap: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.timing_safe_compare: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.transfer: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.unset: BACKLOG / missing-reference/ in port, not in reference
signalwire.swml.service.Service.user_event: BACKLOG / missing-reference/ in port, not in reference
