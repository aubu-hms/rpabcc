cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project (ABCC_host C)
set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME}
    main.c
    )

target_link_libraries(${PROJECT_NAME}
    -lbcm2835
    )

target_sources(${PROJECT_NAME} PRIVATE
    "abcc_adapt/abcc_sys_adapt.c"
    "abcc_adapt/abcc_time.c"
    "abcc_drv/src/abcc_cmd_seq.c"
    "abcc_drv/src/abcc_debug_err.c"
    "abcc_drv/src/abcc_handler.c"
    "abcc_drv/src/abcc_link.c"
    "abcc_drv/src/abcc_mem.c"
    "abcc_drv/src/abcc_remap.c"
    "abcc_drv/src/abcc_seg.c"
    "abcc_drv/src/abcc_setup.c"
    "abcc_drv/src/abcc_timer.c"
    "abcc_drv/src/par/abcc_handler_par.c"
    "abcc_drv/src/par/abcc_par_drv.c"
    "abcc_drv/src/par30/abcc_handler_par30.c"
    "abcc_drv/src/par30/abcc_par30_drv.c"
    "abcc_drv/src/serial/abcc_crc16.c"
    "abcc_drv/src/serial/abcc_handler_ser.c"
    "abcc_drv/src/serial/abcc_serial_drv.c"
    "abcc_drv/src/spi/abcc_crc32.c"
    "abcc_drv/src/spi/abcc_handler_spi.c"
    "abcc_drv/src/spi/abcc_spi_drv.c"
    "abcc_obj/ad_obj.c"
    "abcc_obj/anb_fsi_obj.c"
    "abcc_obj/app_fsi_obj.c"
    "abcc_obj/app_obj.c"
    "abcc_obj/asm_obj.c"
    "abcc_obj/etn_obj.c"
    "abcc_obj/mqtt_obj.c"
    "abcc_obj/nw_obj/bac.c"
    "abcc_obj/nw_obj/ccl.c"
    "abcc_obj/nw_obj/cfn.c"
    "abcc_obj/nw_obj/ciet.c"
    "abcc_obj/nw_obj/cop.c"
    "abcc_obj/nw_obj/dev.c"
    "abcc_obj/nw_obj/dpv1.c"
    "abcc_obj/nw_obj/ect.c"
    "abcc_obj/nw_obj/eip.c"
    "abcc_obj/nw_obj/epl.c"
    "abcc_obj/nw_obj/mod.c"
    "abcc_obj/nw_obj/prt.c"
    "abcc_obj/opcua_obj.c"
    "abcc_obj/safe_obj.c"
    "abcc_obj/sync_obj.c"
    "example_app/appl_abcc_handler.c"
    "example_app/appl_adimap_alltypes.c"
    "example_app/appl_adimap_asm.c"
    "example_app/appl_adimap_bacnet.c"
    "example_app/appl_adimap_separate16.c"
    "example_app/appl_adimap_simple16.c"
    "example_app/appl_adimap_speed_example.c"
    "example_app/appl_adimap_sync.c"
    "example_app/appl_adimap_verif.c"
    "example_app/appl_obj_port.c"
    "ext_lib/Logprint/logprint.c"
)

target_include_directories(${PROJECT_NAME}
PRIVATE
${PROJECT_SOURCE_DIR}
abcc_abp/
abcc_adapt/
abcc_drv/inc/
abcc_drv/src/
abcc_obj/
abcc_obj/nw_obj/
example_app/
ext_lib/Logprint/
)
