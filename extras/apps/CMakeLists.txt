cmake_minimum_required (VERSION 2.6)
project (seqan_extras_apps)

seqan_setup_apps(extras_apps)  # Setup flags etc.

seqan_add_all_subdirectories()  # include_directory(*)
