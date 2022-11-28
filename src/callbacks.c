#ifdef IMINCLUDED

static void hangupWrapper(struct SSockets_ctx* ctx) {
	if(hangupCallback == NULL)
		return;

	CtxObj* wrapped = toWrapped(ctx);

	// Call
	PyObject* arglist = Py_BuildValue("(O)", wrapped);
	PyObject* result = PyObject_CallObject(hangupCallback, arglist);
	Py_DECREF(arglist);
	Py_XDECREF(result);
	if(result == NULL) {
		printf("pyssockets: callback for hangup threw an exception!\n");
		PyErr_Print();
	}

	fromWrapped(wrapped, ctx);
}

static PyObject* setHangupCallback(PyObject* self, PyObject* args) {
	if(!PyArg_ParseTuple(args, "O", &hangupCallback)) {
		PyErr_SetString(PyExc_TypeError, "no callback?");
		return NULL;
	} else if(!PyCallable_Check(hangupCallback)) {
		PyErr_SetString(PyExc_TypeError, "argument is not callable");
		return NULL;
	}
	Py_INCREF(hangupCallback);

	SSockets_setHangupCallback(hangupWrapper);
	Py_RETURN_NONE;
}



static void timeoutWrapper(struct SSockets_ctx* ctx) {
	if(timeoutCallback == NULL)
		return;

	CtxObj* wrapped = toWrapped(ctx);

	// Call
	PyObject* arglist = Py_BuildValue("(O)", wrapped);
	PyObject* result = PyObject_CallObject(timeoutCallback, arglist);
	Py_DECREF(arglist);
	Py_XDECREF(result);
	if(result == NULL) {
		printf("pyssockets: callback for timeout threw an exception!\n");
		PyErr_Print();
	}

	fromWrapped(wrapped, ctx);
}

static PyObject* setTimeoutCallback(PyObject* self, PyObject* args) {
	if(!PyArg_ParseTuple(args, "O", &timeoutCallback)) {
		PyErr_SetString(PyExc_TypeError, "no callback?");
		return NULL;
	} else if(!PyCallable_Check(timeoutCallback)) {
		PyErr_SetString(PyExc_TypeError, "argument is not callable");
		return NULL;
	}
	Py_INCREF(timeoutCallback);

	SSockets_setHangupCallback(timeoutWrapper);
	Py_RETURN_NONE;
}



static void destroyWrapper(struct SSockets_ctx* ctx) {
	if(destroyCallback != NULL) {
		CtxObj* wrapped = toWrapped(ctx);

		// Call
		PyObject* arglist = Py_BuildValue("(O)", wrapped);
		PyObject* result = PyObject_CallObject(destroyCallback, arglist);
		Py_DECREF(arglist);
		Py_XDECREF(result);
		if(result == NULL) {
			printf("pyssockets: callback for destroy threw an exception!\n");
			PyErr_Print();
		}

		fromWrapped(wrapped, ctx);
	}

	// Free data
	if(ctx->data) {
		struct Data* data = ctx->data;
		Py_XDECREF(data->anything);
		Py_XDECREF(data->socket);
		free(data);
	}
}

static PyObject* setDestroyCallback(PyObject* self, PyObject* args) {
	if(!PyArg_ParseTuple(args, "O", &destroyCallback)) {
		PyErr_SetString(PyExc_TypeError, "no callback?");
		return NULL;
	} else if(!PyCallable_Check(destroyCallback)) {
		PyErr_SetString(PyExc_TypeError, "argument is not callable");
		return NULL;
	}
	Py_INCREF(destroyCallback);

	SSockets_setHangupCallback(destroyWrapper);
	Py_RETURN_NONE;
}

#endif
