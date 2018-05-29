#pragma once

#include <afxwin.h>
#include <afxcmn.h>

#include "resource.h"

#include "MainWindow.h"

#ifdef IDE_RUN
#define INPUT_FILES_DIR "src\\"
#else
#define INPUT_FILES_DIR "res\\"
#endif