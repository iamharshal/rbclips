#include <stdlib.h>
#include "clips/clips.h"
#include "rbclips.h"
#include "rbenvironment.h"

/* Definitions */
VALUE cl_cEnvironment;
VALUE cl_vEnvironments;

/**
 * Will create new environment - this class is a little bit exception,
 * newly created environment is directly inserted into CLIPS - there is no
 * save() method.
 */
VALUE cl_environment_new(VALUE self)
{
  cl_sEnvironmentWrap *wrap = calloc( 1, sizeof(*wrap) );
  
  wrap->ptr = CreateEnvironment();

  VALUE ret = Data_Wrap_Struct(self, NULL, free, wrap);
  rb_obj_call_init(ret, 0, NULL);

  // Saving newly created environment into global list
  rb_ary_push(cl_vEnvironments, ret);

  return ret;
}

/**
 * Basically return string representation of pointer to environment data.
 * Not so usefull at all...
 */
VALUE cl_environment_to_s(VALUE self)
{
  cl_sEnvironmentWrap *wrap = DATA_PTR(self);

  return rb_sprintf("Environment:%X", (unsigned int)wrap->ptr);
}

/**
 * Returns copy of internal Environment list
 */
VALUE cl_environment_all()
{
  return rb_ary_dup(cl_vEnvironments);
}

/**
 * Set this environment at current one. If environment is not valid
 * than it raise exception.
 */
VALUE cl_environment_set_current(VALUE self)
{
  cl_sEnvironmentWrap *wrap = DATA_PTR(self);

  // ToDo: Validity check
  // 1) Is valid?
  // 2) Is not active?

  SetCurrentEnvironment(wrap->ptr);

  return Qtrue;
}

/**
 * Return current environment
 */
VALUE cl_environment_current()
{
  cl_sEnvironmentWrap *wrap = calloc( 1, sizeof(*wrap) );
  
  wrap->ptr = GetCurrentEnvironment();

  VALUE ret = Data_Wrap_Struct(cl_cEnvironment, NULL, free, wrap);
  rb_obj_call_init(ret, 0, NULL);

  return ret;
}

/*
 * Create copy of ruby object - the environment itself will be *not*
 * copyed! <objects after that will describe identical environment>
 */
VALUE cl_environment_clone(VALUE self)
{
  cl_sEnvironmentWrap *selfwrap = DATA_PTR(self);
  cl_sEnvironmentWrap *wrap = calloc( 1, sizeof(*wrap) );
  
  wrap->ptr = selfwrap->ptr;

  VALUE ret = Data_Wrap_Struct(cl_cEnvironment, NULL, free, wrap);
  rb_obj_call_init(ret, 0, NULL);

  return ret;
}

/**
 * Return true or false, base on whether the different environment object
 * describe the same CLIPS environment.
 */
VALUE cl_environment_equal(VALUE self, VALUE other)
{
  // Check if other is Environment instance as well

  cl_sEnvironmentWrap *selfwrap = DATA_PTR(self);
  cl_sEnvironmentWrap *otherwrap = DATA_PTR(other);

  return (selfwrap->ptr == otherwrap->ptr) ? Qtrue : Qfalse;
}
