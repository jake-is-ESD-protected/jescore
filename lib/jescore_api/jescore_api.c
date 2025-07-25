#include "jescore_api.h"
#include "core.h"
#include "job_driver.h"
#include "job_names.h"
#include "base_jobs.h"


jes_err_t jes_init(void){
    return __core_init();
}

#ifdef BUILD_FOR_STM32

void jes_dispatch(void){
    vTaskStartScheduler();
}

#endif // BUILD_FOR_STM32


jes_err_t jes_register_job(const char* name,
                       uint32_t mem_size,
                       uint8_t priority,
                       void (*function)(void* p),
                       uint8_t is_loop){
    return __job_register_job(name,
                              mem_size,
                              priority,
                              function,
                              is_loop,
                              e_role_user);
    }


jes_err_t jes_launch_job(const char* name){
    return __job_launch_job_by_name(name, e_origin_api);
}


jes_err_t jes_launch_job_args(const char* name, const char* args){
    return __job_launch_job_by_name_args(name, e_origin_api, args);
}


jes_err_t jes_register_and_launch_job(const char* name,
                                  uint32_t mem_size,
                                  uint8_t priority,
                                  void (*function)(void* p),
                                  uint8_t is_loop){
    jes_err_t stat = __job_register_job(name,
                                        mem_size,
                                        priority,
                                        function,
                                        is_loop,
                                        e_role_user);
    if(stat != e_err_no_err){ return stat; }
    return __job_launch_job_by_name(name, e_origin_api);
}


jes_err_t jes_job_set_args(char* s){
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return e_err_is_zero; }
    return __job_set_args(s, pj);
}


char* jes_job_get_args(void){
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return NULL; }
    return __job_get_args(pj);
}


jes_err_t jes_job_set_param(void* p){
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return e_err_is_zero; }
    return __job_set_param(p, pj);
}


void* jes_job_get_param(void){
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return NULL; }
    return __job_get_param(pj);
}


void jes_notify_job(const char* name, void* notification){
    __job_notify_generic(__job_get_job_by_name(name), notification, 0);
}


void jes_notify_job_ISR(const char* name, void* notification){
    __job_notify_generic(__job_get_job_by_name(name), notification, 1);
}


void* jes_wait_for_notification(void){
    return __job_sleep_until_notified_generic();
}