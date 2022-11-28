#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <ssockets.h>

static void addConstants(PyObject* m) {
	PyObject* obj = PyLong_FromLong(SSockets_RET_OK);
	PyModule_AddObject(m, "RET_OK", obj);

	obj = PyLong_FromLong(SSockets_RET_READ);
	PyModule_AddObject(m, "RET_READ", obj);

	obj = PyLong_FromLong(SSockets_RET_WRITE);
	PyModule_AddObject(m, "RET_WRITE", obj);

	obj = PyLong_FromLong(SSockets_RET_ERROR);
	PyModule_AddObject(m, "RET_ERROR", obj);
	PyModule_AddObjectRef(m, "RET_FINISHED", obj);
}

static PyObject* sockett;

#define IMINCLUDED

// Types
#include "src/Ctx.c"

// Methods
#include "src/states.c"
#include "src/callbacks.c"

static PyMethodDef PyssocketsMethods[] = {
	{"addState", addState, METH_VARARGS, "Add a new state"},
	{"setHangupCallback", setHangupCallback, METH_VARARGS, "Set hangup callback"},
	{"setTimeoutCallback", setTimeoutCallback, METH_VARARGS, "Set timeout callback"},
	{"setDestroyCallback", setDestroyCallback, METH_VARARGS, "Set destroy callback"},
	{"run", run, METH_VARARGS, "Run"},
	{NULL, NULL, 0, NULL}
};

// --- MODULE ---
static struct PyModuleDef pyssockets = {
	PyModuleDef_HEAD_INIT,
	"pyssockets",
	NULL,
	-1,
	PyssocketsMethods
};

PyMODINIT_FUNC PyInit_pyssockets(void) {
	PyObject* m = PyModule_Create(&pyssockets);
	if(m == NULL)
		return NULL;

	addConstants(m);
	addType(m);

	PyObject* socketm = PyImport_ImportModule("socket");
	sockett = PyObject_GetAttrString(socketm, "socket");
	Py_DECREF(socketm);

	return m;
}
