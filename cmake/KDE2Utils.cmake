include("${CMAKE_CURRENT_LIST_DIR}/kde2_library.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/kde2_kidl.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/kde2_module.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/kde2_icon.cmake")

# The unfortunate global definitions
add_definitions(-DQT_NO_TRANSLATION -DQT_CLEAN_NAMESPACE -DQT_NO_COMPAT -DQT_NO_ASCII_CAST)

function(create_kde2_config_header)
    include(CheckIncludeFiles)
    include(CheckFunctionExists)
    include(CheckStructHasMember)
    include(CheckCSourceCompiles)

    check_include_files(malloc.h HAVE_MALLOC_H)
    check_include_files(string.h HAVE_STRING_H)
    check_include_files(unistd.h HAVE_UNISTD_H)
    check_include_files(sys/stat.h HAVE_SYS_STAT_H)
    check_include_files(sys/stat.h HAVE_S_ISSOCK)
    check_include_files(sys/param.h HAVE_SYS_PARAM_H)
    check_include_files(sysent.h HAVE_SYSENT_H)
    check_include_files(sys/select.h HAVE_SYS_SELECT_H)
    check_include_files(sys/param.h;sys/mount.h HAVE_SYS_MOUNT_H)
    check_include_files(float.h HAVE_FLOAT_H)
    check_include_files(mntent.h HAVE_MNTENT_H)
    check_include_files(sys/mnttab.h HAVE_SYS_MNTTAB_H)
    check_include_files(limits.h HAVE_LIMITS_H)
    check_include_files(sys/ucred.h HAVE_SYS_UCRED_H)
    check_include_files(sys/acl.h ACL_H)

    check_function_exists("getdomainname" HAVE_GETDOMAINNAME_PROTO)
    check_function_exists("gethostname" HAVE_GETHOSTNAME_PROTO)
    check_function_exists("setenv" HAVE_SETENV)
    check_function_exists("unsetenv" HAVE_UNSETENV)
    check_function_exists("stpcpy" HAVE_STPCPY)
    check_function_exists("getaddrinfo" HAVE_GETADDRINFO)
    check_function_exists("random" HAVE_RANDOM)
    check_function_exists("seteuid" HAVE_SETEUID)
    check_function_exists("vsnprintf" HAVE_VSNPRINTF)
    check_function_exists("gettimeofday" HAVE_SYS_TIME_H)
    check_function_exists("isinf" HAVE_FUNC_ISINF)
    check_function_exists("isnan" HAVE_FUNC_ISNAN)
    check_function_exists("finite" HAVE_FUNC_FINITE)
    check_function_exists("setmntent" HAVE_SETMNTENT)
    check_function_exists("getmntinfo" HAVE_GETMNTINFO)
    list(APPEND CMAKE_REQUIRED_LIBRARIES acl)
    check_function_exists("acl_get_tag_type" HAVE_ACL_GET_TAG_TYPE)

    check_struct_has_member("struct addrinfo" ai_addrlen "netdb.h" HAVE_STRUCT_ADDRINFO LANGUAGE C)
    check_struct_has_member("struct sockaddr_in6" sin6_port "netinet/in.h" HAVE_SOCKADDR_IN6 LANGUAGE C)
    check_struct_has_member("struct sockaddr_in6" sin6_scope_id "netinet/in.h" HAVE_SOCKADDR_IN6_SCOPE_ID LANGUAGE C)
    check_struct_has_member("struct sockaddr" sa_len "sys/socket.h" HAVE_SOCKADDR_SA_LEN LANGUAGE C)
    check_struct_has_member("struct tm" tm_sec "time.h;sys/time.h" TIME_WITH_SYS_TIME LANGUAGE C)

    if(ACL_H AND HAVE_ACL_GET_TAG_TYPE)
        set(USE_POSIX_ACL TRUE CACHE BOOL "Using posix ACL")
    endif()

    set(KDE_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
    set(KDE_COMPILING_OS ${CMAKE_HOST_SYSTEM_NAME})
    set(KDE_DISTRIBUTION_TEXT "Restoration Project")

    configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)
    include_directories(${PROJECT_BINARY_DIR})
    add_definitions(-DHAVE_CONFIG_H)
endfunction()

set(KDE2_SERVICETYPESDIR ${CMAKE_INSTALL_DATADIR}/servicetypes CACHE STRING "KDE2 ServiceTypes dir")
set(KDE2_SERVICESDIR ${CMAKE_INSTALL_DATADIR}/services CACHE STRING "KDE2 Services dir")
set(KDE2_DATADIR ${CMAKE_INSTALL_DATADIR} CACHE STRING "KDE2 Data dir")
set(KDE2_HTMLDIR ${CMAKE_INSTALL_DOCDIR}/HTML CACHE STRING "KDE2 Doc dir")

