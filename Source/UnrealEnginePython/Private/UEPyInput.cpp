#include "UnrealEnginePythonPrivatePCH.h"


PyObject *py_ue_is_input_key_down(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *key;
	int controller_id = 0;
	if (!PyArg_ParseTuple(args, "s|i:is_input_key_down", &key, &controller_id)) {
		return NULL;
	}

	UWorld *world = ue_get_uworld(self);
	if (!world)
		return PyErr_Format(PyExc_Exception, "unable to retrieve UWorld from uobject");

	APlayerController *controller = UGameplayStatics::GetPlayerController(world, controller_id);
	if (!controller)
		return PyErr_Format(PyExc_Exception, "unable to retrieve controller %d", controller_id);

	if (controller->IsInputKeyDown(key)) {
		Py_INCREF(Py_True);
		return Py_True;
	}

	Py_INCREF(Py_False);
	return Py_False;
}

PyObject *py_ue_was_input_key_just_pressed(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *key;
	int controller_id = 0;
	if (!PyArg_ParseTuple(args, "s|i:was_input_key_just_pressed", &key, &controller_id)) {
		return NULL;
	}

	UWorld *world = ue_get_uworld(self);
	if (!world)
		return PyErr_Format(PyExc_Exception, "unable to retrieve UWorld from uobject");

	APlayerController *controller = UGameplayStatics::GetPlayerController(world, controller_id);
	if (!controller)
		return PyErr_Format(PyExc_Exception, "unable to retrieve controller %d", controller_id);

	if (controller->WasInputKeyJustPressed(key)) {
		Py_INCREF(Py_True);
		return Py_True;
	}

	Py_INCREF(Py_False);
	return Py_False;
}

PyObject *py_ue_is_action_pressed(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *key;
	int controller_id = 0;
	if (!PyArg_ParseTuple(args, "s|i:is_action_pressed", &key, &controller_id)) {
		return NULL;
	}

	UWorld *world = ue_get_uworld(self);
	if (!world)
		return PyErr_Format(PyExc_Exception, "unable to retrieve UWorld from uobject");

	APlayerController *controller = UGameplayStatics::GetPlayerController(world, controller_id);
	if (!controller)
		return PyErr_Format(PyExc_Exception, "unable to retrieve controller %d", controller_id);
	UPlayerInput *input = controller->PlayerInput;
	if (!input)
		goto end;

	for (FInputActionKeyMapping mapping : input->GetKeysForAction(key)) {
		if (controller->WasInputKeyJustPressed(mapping.Key)) {
			Py_INCREF(Py_True);
			return Py_True;
		}
	}

end:
	Py_INCREF(Py_False);
	return Py_False;
}

PyObject *py_ue_was_input_key_just_released(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *key;
	int controller_id = 0;
	if (!PyArg_ParseTuple(args, "s|i:was_input_key_just_released", &key, &controller_id)) {
		return NULL;
	}

	UWorld *world = ue_get_uworld(self);
	if (!world)
		return PyErr_Format(PyExc_Exception, "unable to retrieve UWorld from uobject");

	APlayerController *controller = UGameplayStatics::GetPlayerController(world, controller_id);
	if (!controller)
		return PyErr_Format(PyExc_Exception, "unable to retrieve controller %d", controller_id);

	if (controller->WasInputKeyJustReleased(key)) {
		Py_INCREF(Py_True);
		return Py_True;
	}

	Py_INCREF(Py_False);
	return Py_False;
}

PyObject *py_ue_is_action_released(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *key;
	int controller_id = 0;
	if (!PyArg_ParseTuple(args, "s|i:is_action_released", &key, &controller_id)) {
		return NULL;
	}

	UWorld *world = ue_get_uworld(self);
	if (!world)
		return PyErr_Format(PyExc_Exception, "unable to retrieve UWorld from uobject");

	APlayerController *controller = UGameplayStatics::GetPlayerController(world, controller_id);
	if (!controller)
		return PyErr_Format(PyExc_Exception, "unable to retrieve controller %d", controller_id);
	UPlayerInput *input = controller->PlayerInput;
	if (!input)
		goto end;

	for (FInputActionKeyMapping mapping : input->GetKeysForAction(key)) {
		if (controller->WasInputKeyJustReleased(mapping.Key)) {
			Py_INCREF(Py_True);
			return Py_True;
		}
	}

end:
	Py_INCREF(Py_False);
	return Py_False;
}

PyObject *py_ue_enable_input(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	int controller_id = 0;
	if (!PyArg_ParseTuple(args, "|i:enable_input", &controller_id)) {
		return NULL;
	}

	UWorld *world = ue_get_uworld(self);
	if (!world)
		return PyErr_Format(PyExc_Exception, "unable to retrieve UWorld from uobject");

	APlayerController *controller = UGameplayStatics::GetPlayerController(world, controller_id);
	if (!controller)
		return PyErr_Format(PyExc_Exception, "unable to retrieve controller %d", controller_id);

	if (self->ue_object->IsA<AActor>()) {
		((AActor *)self->ue_object)->EnableInput(controller);
	}
	else if (self->ue_object->IsA<UActorComponent>()) {
		((UActorComponent *)self->ue_object)->GetOwner()->EnableInput(controller);
	}
	else {
		return PyErr_Format(PyExc_Exception, "uobject is not an actor or a component");
	}

	Py_INCREF(Py_None);
	return Py_None;

}

PyObject *py_ue_get_input_axis(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *axis_name;
	if (!PyArg_ParseTuple(args, "s:get_input_axis", &axis_name)) {
		return NULL;
	}

	UInputComponent *input = nullptr;

	if (self->ue_object->IsA<AActor>()) {
		input = ((AActor *)self->ue_object)->InputComponent;
	}
	else if (self->ue_object->IsA<UActorComponent>()) {
		input = ((UActorComponent *)self->ue_object)->GetOwner()->InputComponent;
	}
	else {
		return PyErr_Format(PyExc_Exception, "uobject is not an actor or a component");
	}

	if (!input) {
		return PyErr_Format(PyExc_Exception, "no input manager for this uobject");
	}

	return Py_BuildValue("f", input->GetAxisValue(FName(UTF8_TO_TCHAR(axis_name))));

}

PyObject *py_ue_bind_input_axis(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *axis_name;
	if (!PyArg_ParseTuple(args, "s:bind_input_axis", &axis_name)) {
		return NULL;
	}

	UInputComponent *input = nullptr;

	if (self->ue_object->IsA<AActor>()) {
		input = ((AActor *)self->ue_object)->InputComponent;
	}
	else if (self->ue_object->IsA<UActorComponent>()) {
		input = ((UActorComponent *)self->ue_object)->GetOwner()->InputComponent;
	}
	else {
		return PyErr_Format(PyExc_Exception, "uobject is not an actor or a component");
	}

	if (!input) {
		return PyErr_Format(PyExc_Exception, "no input manager for this uobject");
	}

	input->BindAxis(FName(UTF8_TO_TCHAR(axis_name)));

	Py_INCREF(Py_None);
	return Py_None;

}

PyObject *py_ue_show_mouse_cursor(ue_PyUObject * self, PyObject * args) {

	ue_py_check(self);

	bool enabled = true;

	PyObject *is_true = NULL;
	int controller_id = 0;
	if (!PyArg_ParseTuple(args, "|Oi:show_mouse_cursor", &is_true, &controller_id)) {
		return NULL;
	}

	if (is_true && !PyObject_IsTrue(is_true))
		enabled = false;

	UWorld *world = ue_get_uworld(self);
	if (!world)
		return PyErr_Format(PyExc_Exception, "unable to retrieve UWorld from uobject");

	APlayerController *controller = UGameplayStatics::GetPlayerController(world, controller_id);
	if (!controller)
		return PyErr_Format(PyExc_Exception, "unable to retrieve controller %d", controller_id);
	
	controller->bShowMouseCursor = enabled;

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_ue_enable_click_events(ue_PyUObject * self, PyObject * args) {

	ue_py_check(self);

	bool enabled = true;

	PyObject *is_true = NULL;
	int controller_id = 0;
	if (!PyArg_ParseTuple(args, "|Oi:enable_click_events", &is_true, &controller_id)) {
		return NULL;
	}

	if (is_true && !PyObject_IsTrue(is_true))
		enabled = false;

	UWorld *world = ue_get_uworld(self);
	if (!world)
		return PyErr_Format(PyExc_Exception, "unable to retrieve UWorld from uobject");

	APlayerController *controller = UGameplayStatics::GetPlayerController(world, controller_id);
	if (!controller)
		return PyErr_Format(PyExc_Exception, "unable to retrieve controller %d", controller_id);

	controller->bEnableClickEvents = enabled;

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_ue_enable_mouse_over_events(ue_PyUObject * self, PyObject * args) {

	ue_py_check(self);

	bool enabled = true;

	PyObject *is_true = NULL;
	int controller_id = 0;
	if (!PyArg_ParseTuple(args, "|Oi:enable_mouse_over_events", &is_true, &controller_id)) {
		return NULL;
	}

	if (is_true && !PyObject_IsTrue(is_true))
		enabled = false;

	UWorld *world = ue_get_uworld(self);
	if (!world)
		return PyErr_Format(PyExc_Exception, "unable to retrieve UWorld from uobject");


	APlayerController *controller = UGameplayStatics::GetPlayerController(world, controller_id);
	if (controller)
		controller->bEnableMouseOverEvents = enabled;

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_ue_bind_action(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *action_name;
	int key;
	PyObject *py_callable;
	if (!PyArg_ParseTuple(args, "siO:bind_action", &action_name, &key, &py_callable)) {
		return NULL;
	}

	if (!PyCallable_Check(py_callable)) {
		return PyErr_Format(PyExc_Exception, "object is not a callable");
	}

	UInputComponent *input = nullptr;

	if (self->ue_object->IsA<AActor>()) {
		input = ((AActor *)self->ue_object)->InputComponent;
	}
	else if (self->ue_object->IsA<UActorComponent>()) {
		input = ((UActorComponent *)self->ue_object)->GetOwner()->InputComponent;
	}
	else {
		return PyErr_Format(PyExc_Exception, "uobject is not an actor or a component");
	}

	if (!input) {
		return PyErr_Format(PyExc_Exception, "no input manager for this uobject");
	}

	UPythonDelegate *py_delegate = NewObject<UPythonDelegate>();
	py_delegate->SetPyCallable(py_callable);
	py_delegate->AddToRoot();

	// allow the delegate to not be destroyed
	self->ue_property->python_delegates_gc.Add(py_delegate);

	FInputActionBinding input_action_binding(FName(UTF8_TO_TCHAR(action_name)), (const EInputEvent) key);
	input_action_binding.ActionDelegate.BindDelegate(py_delegate, &UPythonDelegate::PyInputHandler);
	input->AddActionBinding(input_action_binding);

	Py_INCREF(Py_None);
	return Py_None;

}

PyObject *py_ue_bind_axis(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *axis_name;
	PyObject *py_callable;
	if (!PyArg_ParseTuple(args, "sO:bind_action", &axis_name, &py_callable)) {
		return NULL;
	}

	if (!PyCallable_Check(py_callable)) {
		return PyErr_Format(PyExc_Exception, "object is not a callable");
	}

	UInputComponent *input = nullptr;

	if (self->ue_object->IsA<AActor>()) {
		input = ((AActor *)self->ue_object)->InputComponent;
	}
	else if (self->ue_object->IsA<UActorComponent>()) {
		input = ((UActorComponent *)self->ue_object)->GetOwner()->InputComponent;
	}
	else {
		return PyErr_Format(PyExc_Exception, "uobject is not an actor or a component");
	}

	if (!input) {
		return PyErr_Format(PyExc_Exception, "no input manager for this uobject");
	}

	UPythonDelegate *py_delegate = NewObject<UPythonDelegate>();
	py_delegate->SetPyCallable(py_callable);
	py_delegate->AddToRoot();

	// allow the delegate to not be destroyed
	self->ue_property->python_delegates_gc.Add(py_delegate);

	FInputAxisBinding input_axis_binding(FName(UTF8_TO_TCHAR(axis_name)));
	input_axis_binding.AxisDelegate.BindDelegate(py_delegate, &UPythonDelegate::PyInputAxisHandler);
	input->AxisBindings.Add(input_axis_binding);

	Py_INCREF(Py_None);
	return Py_None;

}

PyObject *py_ue_bind_key(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	char *key_name;
	int key;
	PyObject *py_callable;
	if (!PyArg_ParseTuple(args, "siO:bind_key", &key_name, &key, &py_callable)) {
		return NULL;
	}

	if (!PyCallable_Check(py_callable)) {
		return PyErr_Format(PyExc_Exception, "object is not a callable");
	}

	UInputComponent *input = nullptr;

	if (self->ue_object->IsA<AActor>()) {
		input = ((AActor *)self->ue_object)->InputComponent;
	}
	else if (self->ue_object->IsA<UActorComponent>()) {
		input = ((UActorComponent *)self->ue_object)->GetOwner()->InputComponent;
	}
	else {
		return PyErr_Format(PyExc_Exception, "uobject is not an actor or a component");
	}

	if (!input) {
		return PyErr_Format(PyExc_Exception, "no input manager for this uobject");
	}

	UPythonDelegate *py_delegate = NewObject<UPythonDelegate>();
	py_delegate->SetPyCallable(py_callable);
	py_delegate->AddToRoot();

	// allow the delegate to not be destroyed
	self->ue_property->python_delegates_gc.Add(py_delegate);

	FInputKeyBinding input_key_binding(FKey(UTF8_TO_TCHAR(key_name)), (const EInputEvent)key);
	input_key_binding.KeyDelegate.BindDelegate(py_delegate, &UPythonDelegate::PyInputHandler);
	input->KeyBindings.Add(input_key_binding);

	Py_INCREF(Py_None);
	return Py_None;

}

PyObject *py_ue_bind_pressed_key(ue_PyUObject *self, PyObject * args) {
	ue_py_check(self);
	char *key_name;
	PyObject *py_callable;
	if (!PyArg_ParseTuple(args, "sO:bind_pressed_key", &key_name, &py_callable)) {
		return NULL;
	}
	return py_ue_bind_key(self, Py_BuildValue("siO", key_name, EInputEvent::IE_Pressed, py_callable));
}

PyObject *py_ue_bind_released_key(ue_PyUObject *self, PyObject * args) {
	ue_py_check(self);
	char *key_name;
	PyObject *py_callable;
	if (!PyArg_ParseTuple(args, "sO:bind_released_key", &key_name, &py_callable)) {
		return NULL;
	}
	return py_ue_bind_key(self, Py_BuildValue("siO", key_name, EInputEvent::IE_Released, py_callable));
}





