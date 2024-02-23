#pragma once

extern "C" {

    int sign_ipa(
            const char *ipa_path,
            const char *key_path,
            const char *mp_path,
            const char *dylib_file_path,
            const char *icon_path,
            const char *tmp_folder_path,
            char *error
    );

}