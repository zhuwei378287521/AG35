
/*
 *  Quectel common at command handler
 *
 *  file name: quectel_common_at_handler.h
 *
 *  Design by: jun.wu
 *
 *  Date: 2016-01-24 
 */

#ifndef __QUECTEL_COMMON_AT_HANDLER__
#define __QUECTEL_COMMON_AT_HANDLER__

#ifdef __cplusplus
extern "C" {
#endif
#include "quectel-features-config.h"//2017.12.04 add command AT+QVERSION by wayne.wei
#include "quectel_at_cmd_defs.h"
#define QUECTEL_COMMON_QMI
#ifdef QUECTEL_POWER_AT

extern int parse_qpowd_parameters(AtCmd *cmd, atfwd_param_s_type *para);

extern void handle_qpowd(void *param, quec_AtCmdResponse *response);
extern void exec_qpowd(quec_AtCmdResponse *response);

#ifdef QUECTEL_DRIVER_ATC_QSCLK	
extern int parse_qsclk_parameters(AtCmd *cmd, atfwd_param_s_type *para);

extern void handle_qsclk(void *param, quec_AtCmdResponse *response);

extern void quec_sleep_read_process(void);
#endif

#ifdef MINIPCIE_MODE_PIN
extern void quectel_qcfg_sleepind_level_set_mode(int mode);
extern void quectel_qcfg_wakeupin_level_set_mode(int mode);
#endif
#endif


#ifdef  QUECTEL_LINUX_PRINT_ATC
extern int parse_qprint_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qprint(void *param, quec_AtCmdResponse *response);
extern void handle_qprint_exec(quec_AtCmdResponse *response);
#endif

#ifdef  QUECTEL_LINUX_SDMOUNT_ATC
extern int parse_qsdmount_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qsdmount(void *param, quec_AtCmdResponse *response);
extern void handleReturnSdmount();
extern void handleRuturn_checkSdmount();
extern void query_qsdmount(quec_AtCmdResponse *response);
#endif

extern int parse_qcfg_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qcfg(void* param, quec_AtCmdResponse *response);

#ifdef QUECTEL_ADC
extern int quec_parse_cbc_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void quec_exec_cbc(quec_AtCmdResponse * response);
extern int quec_parse_qadc_parameters(AtCmd *cmd, atfwd_param_s_type *para); //Added by running 2016-06-06, for adc function
extern int quec_parse_qadctemp_parameters(AtCmd *cmd, atfwd_param_s_type *para);//Added by sundy 2016-08-27, for adctemp function
extern void handle_qadc(void *parm, quec_AtCmdResponse * response);//Added by running 2016-06-06, for adc function
extern void handle_qadctemp(void *parm, quec_AtCmdResponse * response);//Added by sundy 2016-08-27, for adctemp function
#endif
#ifdef QUECTEL_LINUX_GPS_UARTDEBUG
extern int quec_parse_qgpscfg_parameters(AtCmd *cmd, atfwd_param_s_type *para);//Added by sundy 2016-09-18, for outport debuguart function
extern void handle_qgpscfg(void *parm, quec_AtCmdResponse * response);//Added by sundy 2016-09-18, for outport debuguart function
#endif

#ifdef  QUECTEL_FASTBOOT_ATC
extern int parse_qfastboot_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qfastboot_exec(quec_AtCmdResponse * response);


#endif

#ifdef QUECTEL_QCFG_ITEM
extern int parse_qcfg_parameters_Ex(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qcfg_Ex(void* param, quec_AtCmdResponse *response);
#endif

#ifdef QUECTEL_AT_Q_VERSION //2017.12.04 add command AT+QVERSION by wayne.wei
void query_qversion(quec_AtCmdResponse *response);
#endif
#ifdef QUECTEL_AT_SUBSYS_VERSION  //Wayne.wei -2018/5/25 :add an AT command :AT+QSUBSYSVER to query subsystem version of TZ/SBL/RPM 
void query_subsys_version(quec_AtCmdResponse *response);
#endif

#ifdef QUECTEL_QTEMP
int parse_qtemp_parameters(AtCmd *cmd, atfwd_param_s_type *para);
void handle_qtemp_exec(void *param, quec_AtCmdResponse * response);
int parse_qtempdbg_parameters(AtCmd *cmd, atfwd_param_s_type *para);
void handle_qtempdbg(quec_AtCmdResponse * response);
int parse_qtempdbglvl_parameters(AtCmd *cmd, atfwd_param_s_type *para);
void handle_qtempdbglvl_exec(void *param, quec_AtCmdResponse * response);
#endif

extern int parse_qlinuxcpu_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void query_qlinuxcpu_exec(quec_AtCmdResponse *response);
#ifdef QUECTEL_COMMON_QMI
void quectel_process_sclk_value_changge_sigusr1_event(void);
void quectel_process_sclk_value_changge_sigusr2_event(void);
#endif

#ifdef QUECTEL_QAPNMEAFWD
int parse_qapnmeafwd_parameters(AtCmd *cmd, atfwd_param_s_type *para);
void handle_qapnmeafwd_query(quec_AtCmdResponse * response);
void handle_qapnmeafwd_exec(void *param, quec_AtCmdResponse * response);
#endif


#ifdef __cplusplus
}
#endif
#endif /* __QUECTEL_AUDIO_AT_HANDLER__ */

