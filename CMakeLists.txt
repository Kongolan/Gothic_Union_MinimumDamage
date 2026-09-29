cmake_minimum_required(VERSION 3.20)

# --- FIX FÜR CMAKE 4.0+ KOMPATIBILITÄT DER UNION API ---
# Erlaubt alte Abhängigkeiten (wie OGG/ZLIB), die CMake < 3.5 nutzen
set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "Minimum policy version" FORCE)
# Erlaubt relative Pfade für externe Targets (behebt CMP0076)
set(CMAKE_POLICY_DEFAULT_CMP0076 NEW CACHE STRING "Policy default" FORCE)
# Verhindert, dass Variablen durch option() überschrieben werden (behebt CMP0077)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW CACHE STRING "Policy default" FORCE)
# Erlaubt das Linken von Bibliotheken außerhalb des aktuellen Ordners (behebt CMP0079 für detours)
set(CMAKE_POLICY_DEFAULT_CMP0079 NEW CACHE STRING "Policy default" FORCE)
# -------------------------------------------------------

project(Union_MinimumDamage VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(WARNING "Building for 64-bit. Gothic plugins should be 32-bit (x86).")
endif()

# Fetch Official Union API (lädt Gothic API automatisch mit herunter)
include(FetchContent)
FetchContent_Declare(
  union-api
  GIT_REPOSITORY https://gitlab.com/union-framework/union-api.git
  GIT_TAG main
)
FetchContent_MakeAvailable(union-api)

add_library(Union_MinimumDamage SHARED src/Plugin.cpp)

# Verknüpft unsere Mod mit der API (setzt Include-Pfade jetzt vollautomatisch!)
target_link_libraries(Union_MinimumDamage PRIVATE union_api_lib)

# Sorgt dafür, dass am Ende eine .dll ohne Präfix entsteht
set_target_properties(Union_MinimumDamage PROPERTIES PREFIX "" SUFFIX ".dll")