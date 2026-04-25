#include <array>

std::array ntdllExports = {
    "wine_server_call",
    "wine_server_fd_to_handle",
    "wine_server_handle_to_fd",
    "__wine_unix_spawnvp",
    "__wine_ctrl_routine",
    "__wine_syscall_dispatcher",
    "__wine_unix_call_dispatcher",
    "__wine_unix_call_dispatcher_arm64ec",
    "__wine_unixlib_handle",
    "__wine_dbg_write",
    "__wine_dbg_get_channel_flags",
    "__wine_dbg_header",
    "__wine_dbg_output",
    "__wine_dbg_strdup",
    "wine_get_version",
    "wine_get_build_id",
    "wine_get_host_version"
};

std::array kernel32Exports = {
    "wine_get_unix_file_name",
    "wine_get_dos_file_name"
};