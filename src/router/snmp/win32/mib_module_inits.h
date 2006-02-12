/* This file is automatically generated by configure.  Do not modify by hand. */
  if (should_init("system_mib")) init_system_mib();
  if (should_init("sysORTable")) init_sysORTable();
#ifdef HAVE_WIN32_PLATFORM_SDK
  if (should_init("at")) init_at();
  if (should_init("interfaces")) init_interfaces();
  if (should_init("tcp")) init_tcp();
  if (should_init("icmp")) init_icmp();
  if (should_init("ip")) init_ip();
  if (should_init("udp")) init_udp();
  if (should_init("var_route")) init_var_route();
#endif
  if (should_init("snmp_mib")) init_snmp_mib();
  if (should_init("vacm_vars")) init_vacm_vars();
  if (should_init("proc")) init_proc();
  if (should_init("versioninfo")) init_versioninfo();
  if (should_init("pass")) init_pass();
  if (should_init("pass_persist")) init_pass_persist();
  if (should_init("disk")) init_disk();
  if (should_init("loadave")) init_loadave();
  if (should_init("extensible")) init_extensible();
  if (should_init("errormib")) init_errormib();
  if (should_init("file")) init_file();
  if (should_init("dlmod")) init_dlmod();
  if (should_init("proxy")) init_proxy();
  if (should_init("snmpEngine")) init_snmpEngine();
  if (should_init("snmpMPDStats")) init_snmpMPDStats();
  if (should_init("usmStats")) init_usmStats();
  if (should_init("usmUser")) init_usmUser();
  if (should_init("snmpNotifyTable")) init_snmpNotifyTable();
  if (should_init("snmpNotifyFilterTable")) init_snmpNotifyFilterTable();
  if (should_init("snmpNotifyFilterProfileTable")) init_snmpNotifyFilterProfileTable();
  if (should_init("snmpTargetAddrEntry")) init_snmpTargetAddrEntry();
  if (should_init("snmpTargetParamsEntry")) init_snmpTargetParamsEntry();
  if (should_init("target_counters")) init_target_counters();
  if (should_init("nsTransactionTable")) init_nsTransactionTable();
  if (should_init("nsModuleTable")) init_nsModuleTable();
  /* if (should_init("subagent")) init_subagent(); */
  if (should_init("override")) init_override();
  if (should_init("vacm_context")) init_vacm_context();
  if (should_init("agentx_config")) init_agentx_config();
