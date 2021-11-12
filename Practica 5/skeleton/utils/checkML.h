#pragma once
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC_
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW_ new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW_
#endif
#endif