#ifdef IMINCLUDED

struct Data {
	PyObject* anything;
	PyObject* socket;
};

typedef struct {
	PyObject_HEAD
	// These are now the same "public" fields as in SSockets_ctx
	PyObject* state;
	PyObject* fd;
	PyObject* addr;
	PyObject* timeout;
	PyObject* disarm;

	PyObject* socket;
	PyObject* data;
} CtxObj;

static void Ctx_dealloc(CtxObj* self) {
	Py_XDECREF(self->state);
	Py_XDECREF(self->fd);
	Py_XDECREF(self->addr);
	Py_XDECREF(self->timeout);
	Py_XDECREF(self->disarm);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyMemberDef Ctx_members[] = {
	{"state", T_OBJECT_EX, offsetof(CtxObj, state), 0, "State ID"},
	{"fd", T_OBJECT_EX, offsetof(CtxObj, fd), 0, "File descriptor"},
	{"addr", T_OBJECT_EX, offsetof(CtxObj, addr), 0, "Client's address"},
	{"timeout", T_OBJECT_EX, offsetof(CtxObj, timeout), 0, "Timeout in seconds"},
	{"disarm", T_OBJECT_EX, offsetof(CtxObj, disarm), 0, "Disarm the timer"},
	{"socket", T_OBJECT_EX, offsetof(CtxObj, socket), 0, "Socket object of fd"},
	{"data", T_OBJECT_EX, offsetof(CtxObj, data), 0, "Whatever you want"},
	{NULL}
};

static PyTypeObject Ctx = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "pyssockets.Ctx",
	.tp_doc = PyDoc_STR("Context object. Do not instantiate"),
	.tp_basicsize = sizeof(CtxObj),
	.tp_itemsize = 0,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_dealloc = (destructor)Ctx_dealloc,
	.tp_members = Ctx_members
};

static void addType(PyObject* m) {
	if(PyType_Ready(&Ctx) < 0)
		return;
	Py_INCREF(&Ctx);
	PyModule_AddObjectRef(m, "Ctx", (PyObject*)&Ctx);
}

#endif
