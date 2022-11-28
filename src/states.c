#ifdef IMINCLUDED

#define MAX_IP_LENGTH sizeof("000.000.000.000\0")

static PyObject** callbacks = NULL;
static size_t ncallbacks = 0;
static PyObject* hangupCallback = NULL;
static PyObject* timeoutCallback = NULL;
static PyObject* destroyCallback = NULL;

static CtxObj* toWrapped(struct SSockets_ctx* ctx) {
	// Build the Ctx object, manually (no init)
	CtxObj* wrapped = (CtxObj*)(Ctx.tp_alloc(&Ctx, 0));
	wrapped->state = PyLong_FromLong(ctx->state);
	wrapped->fd = PyLong_FromLong(ctx->fd);

	char* ip = malloc(MAX_IP_LENGTH);
	inet_ntop(AF_INET, &(ctx->addr.sin_addr), ip, MAX_IP_LENGTH);
	wrapped->addr = PyUnicode_FromString(ip);
	free(ip);

	wrapped->timeout = PyLong_FromLong(ctx->timeout);
	wrapped->disarm = PyBool_FromLong(ctx->disarm);

	struct Data* data = ctx->data;
	if(data == NULL) {
		// Data wrapper
		data = ctx->data = malloc(sizeof(struct Data));

		// "anything"; that is, actual usable data by the user
		Py_INCREF(Py_None);
		data->anything = Py_None;

		// A socket, wrapper for ctx->fd (much more comfortable)
		PyObject* args = Py_BuildValue("()");
		PyObject* kwargs = PyDict_New();
		PyObject* fileno = PyUnicode_FromString("fileno");
		PyDict_SetItem(kwargs, fileno, wrapped->fd);
		Py_DECREF(fileno);
		data->socket = PyObject_Call(sockett, args, kwargs);
		Py_DECREF(kwargs);
		Py_DECREF(args);
	}

	wrapped->socket = data->socket;
	wrapped->data = data->anything;
	return wrapped;
}

static void fromWrapped(CtxObj* wrapped, struct SSockets_ctx* ctx) {
	// Set values in ctx
	ctx->state = PyLong_AsLong(wrapped->state);
	ctx->fd = PyLong_AsLong(wrapped->fd);
	ctx->timeout = PyLong_AsLong(wrapped->timeout);
	ctx->disarm = (wrapped->disarm == Py_True);

	struct Data* data = ctx->data;
	data->anything = wrapped->data;

	// Destroy wrapped object
	Py_DECREF(wrapped);
}

static int stateWrapper(struct SSockets_ctx* ctx) {
	CtxObj* wrapped = toWrapped(ctx);

	// Call
	PyObject* cb = callbacks[ctx->state];
	int ret = SSockets_RET_ERROR;
	PyObject* arglist = Py_BuildValue("(O)", wrapped);
	PyObject* result = PyObject_CallObject(cb, arglist);
	Py_DECREF(arglist);
	if(result) {
		ret = PyLong_AsLong(result);
		Py_DECREF(result);
	} else {
		printf("pyssockets: callback for state %lu threw an exception!\n", ctx->state);
		PyErr_Print();
		printf("pyssockets: returning RET_ERROR\n");
	}

	fromWrapped(wrapped, ctx);
	return ret;
}

static PyObject* addState(PyObject* self, PyObject* args) {
	PyObject* cb;
	if(!PyArg_ParseTuple(args, "O", &cb)) {
		PyErr_SetString(PyExc_TypeError, "no callback?");
		return NULL;
	} else if(!PyCallable_Check(cb)) {
		PyErr_SetString(PyExc_TypeError, "argument is not callable");
		return NULL;
	}
	Py_INCREF(cb);

	callbacks = realloc(callbacks, (ncallbacks+1) * sizeof(PyObject*));
	callbacks[ncallbacks] = cb;

	SSockets_addState(stateWrapper);
	return PyLong_FromLong(ncallbacks++);
}

static PyObject* run(PyObject* self, PyObject* args) {
	const char* ip;
	uint16_t port;
	size_t nthreads;
	if(!PyArg_ParseTuple(args, "sHk", &ip, &port, &nthreads)) {
		PyErr_SetString(PyExc_TypeError, "bad arguments");
		return NULL;
	}

	SSockets_run(ip, port, nthreads);

	PyErr_SetString(PyExc_TypeError, "SSockets_run returned!?");
	return NULL;
}

#endif
