#include <RuneStone/Object.h>
#include <Dirt/SimpleStruct.h>
#include <Dirt/StructImplementor.h>
#include <stdlib.h>

/* To test how some data *should* be serialized, check using the
   following Python function:
 
def testSerialize(data):
    import Grimoire, StringIO
    res = StringIO.StringIO()
    Grimoire.Utils.Serialize.Writer.write(res,
        Grimoire.Utils.Serialize.Writer.contract(
            data,
	    Grimoire.Utils.Serialize.RPC.ObjectMappedExtension(
                Grimoire.Utils.Serialize.RPC.stdObjectMap()).serialize))
    return res.getvalue()
 */

void RuneStone_Struct_Class_free(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_ClassStruct *cls = (RuneStone_Struct_ClassStruct *) item;
  size_t path;

  for (path = 0; path < cls->paths; path++)
   free(cls->path[path]);
  free(cls->path);
 }

/* Grimoire.Types.AnnotatedValue
   ==> <'Type', [['__builtin__', 'object'],
  	         ['Grimoire', 'Types', 'Composable', 'Composable'],
  		 ['Grimoire', 'Types', 'Composable', 'Mapping'],
  		 ['Grimoire', 'Types', 'Composable', 'AnnotatedValue']]>
 */
Dirt_Struct *RuneStone_Struct_Class_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_ClassStruct *cls = (RuneStone_Struct_ClassStruct *) item;
  Dirt_Struct *lst, *pth;
  size_t i, j, k;

  lst = strct();
  for (i = 0; i < cls->paths; i++)
   {
    pth = strct();
    for (j=k=0; cls->path[i][k]; j = k + 1)
     {
      for (k = j; cls->path[i][k] && cls->path[i][k] != '.'; k++);
      pth = append(pth, blob(cls->path[i] + j, k - j));
     }
    lst = append(lst, list(pth));
   }
  return type(append(append(strct(),
			    STR("Type")),
		     list(lst)));
 }

void RuneStone_Struct_Instance_free(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_InstanceStruct *inst = (RuneStone_Struct_InstanceStruct *) item;
  size_t member;
  
  inst->class->type->decref(session, inst->class);
  for (member = 0; member < inst->members; member++)
   {
    free(inst->name[member]);
    inst->value[member]->type->decref(session, inst->value[member]);
   }
  free(inst->name);
  free(inst->value);
 }

/* class X(object): pass
   x = X()
   x.foo = 1
   ==> <'Object', (<'Type', [['__builtin__', 'object'],
                             ['__main__', 'X']]>,
                   {'foo': 1})>
 */
Dirt_Struct *RuneStone_Struct_Instance_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_InstanceStruct *inst = (RuneStone_Struct_InstanceStruct *) item;
  Dirt_Struct *members;
  size_t i;

  members = strct();
  for (i = 0; i < inst->members; i++)
   members = append(members,
		    keyval(str(inst->name[i]),
			   inst->value[i]->type->contract(session, inst->value[i])));
  members = dict(members);
  return type(append(append(strct(),
			    STR("Object")),
		     list(append(append(strct(),
					inst->class->type->contract(session, inst->class)),
				 members))));
 }

void RuneStone_Struct_BuiltinBasedInstance_free(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_BuiltinBasedInstanceStruct *inst = (RuneStone_Struct_BuiltinBasedInstanceStruct *) item;
  size_t base;
  
  for (base = 0; base < inst->baseValues; base++)
   inst->baseValue[base]->type->decref(session, inst->baseValue[base]);
  free(inst->baseValue);
  RuneStone_StructType_Instance.free(session, item);
 }

/* class X(dict): pass
   x = X({"bar":2})
   x.foo = 1
   ==> <'Object', (<'Type', [['__builtin__', 'object'],
                             ['__builtin__', 'dict'],
                             ['__main__', 'X']]>,
                   ([{'bar': 2}],
                    {'foo': 1}))>
 */
Dirt_Struct *RuneStone_Struct_BuiltinBasedInstance_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_InstanceStruct *inst = (RuneStone_Struct_InstanceStruct *) item;
  RuneStone_Struct_BuiltinBasedInstanceStruct *binst = (RuneStone_Struct_BuiltinBasedInstanceStruct *) item;
  Dirt_Struct *baseValues;
  Dirt_Struct *members;
  size_t i;

  members = strct();
  for (i = 0; i < inst->members; i++)
   members = append(members,
		    keyval(str(inst->name[i]),
			   inst->value[i]->type->contract(session, inst->value[i])));
  members = dict(members);

  baseValues = strct();
  for (i = 0; i < binst->baseValues; i++)
   baseValues = append(baseValues,
		       binst->baseValue[i]->type->contract(session, binst->baseValue[i]));
  baseValues = list(baseValues);

  return type(append(append(strct(),
			    STR("Object")),
		     list(append(append(strct(),
					inst->class->type->contract(session, inst->class)),
				 list(append(append(strct(),
				                    baseValues),
					     members))))));
 }

void RuneStone_Struct_TypeInstance_free(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_TypeInstanceStruct *inst = (RuneStone_Struct_TypeInstanceStruct *) item;
  size_t base;
  
  for (base = 0; base < inst->baseClasses; base++)
   inst->baseClass[base]->type->decref(session, inst->baseClass[base]);
  free(inst->baseClass);
  free(inst->name);
  RuneStone_StructType_Instance.free(session, item);
 }

/* class dummy(type):
       x = 1
   X = dummy
   del dummy
   ==> <'Object', (<'Type', [['__builtin__', 'object'],
                             ['__builtin__', 'type']]>,
                   ('X',
		    (<'Type', [['__builtin__', 'object'],
		               ['__builtin__', 'type']]>, ),
                    {'x': 1,
		     '__module__': '__main__',
		     '__doc__': None}, ), )>
 */
Dirt_Struct *RuneStone_Struct_TypeInstance_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_InstanceStruct *inst = (RuneStone_Struct_InstanceStruct *) item;
  RuneStone_Struct_TypeInstanceStruct *tinst = (RuneStone_Struct_TypeInstanceStruct *) item;
  Dirt_Struct *baseClasses;
  Dirt_Struct *members;
  size_t i;

  members = strct();
  for (i = 0; i < inst->members; i++)
   members = append(members,
		    keyval(str(inst->name[i]),
			   inst->value[i]->type->contract(session, inst->value[i])));
  members = dict(members);

  baseClasses = strct();
  for (i = 0; i < tinst->baseClasses; i++)
   baseClasses = append(baseClasses,
		  tinst->baseClass[i]->type->contract(session, tinst->baseClass[i]));
  baseClasses = list(baseClasses);

  return type(append(append(strct(),
			    STR("Object")),
		     list(append(append(strct(),
					inst->class->type->contract(session, inst->class)),
				 list(append(append(append(strct(),
							   str(tinst->name)),
				                    baseClasses),
					     members))))));
 }



Dirt_StructType RuneStone_StructType_Class = { sizeof(RuneStone_Struct_ClassStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_Class_free, RuneStone_Struct_Class_contract };
Dirt_StructType RuneStone_StructType_Instance = { sizeof(RuneStone_Struct_InstanceStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_Instance_free, RuneStone_Struct_Instance_contract };
Dirt_StructType RuneStone_StructType_BuiltinBasedInstance = { sizeof(RuneStone_Struct_BuiltinBasedInstanceStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_BuiltinBasedInstance_free, RuneStone_Struct_BuiltinBasedInstance_contract };
Dirt_StructType RuneStone_StructType_TypeInstance = { sizeof(RuneStone_Struct_TypeInstanceStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_TypeInstance_free, RuneStone_Struct_TypeInstance_contract };


Dirt_Struct *RuneStone_Struct_classStruct(Dirt_Session *session, char **path, size_t paths)
 {
  Dirt_Struct *res;
  RuneStone_Struct_ClassStruct *cls;

  if (!path) return NULL;
  if (!(res = Dirt_Struct_init(session, &RuneStone_StructType_Class))) return NULL;
  cls = (RuneStone_Struct_ClassStruct *) res;
  cls->paths = 0;
  if (!(cls->path = malloc(sizeof(char *) * paths)))
   {
    res->type->decref(session, res);
    return NULL;
   }
  for (;cls->paths < paths;cls->paths++)
   {
    if (!(cls->path[cls->paths] = malloc(strlen(path[cls->paths]) + 1)))
     {
      res->type->decref(session, res);
      return NULL;
     }
    strcpy(cls->path[cls->paths], path[cls->paths]);
   }
  return res;
 }

Dirt_Struct *RuneStone_Struct_InstanceStruct_init(Dirt_Session *session, Dirt_StructType *type, Dirt_Struct *class, char **name, Dirt_Struct **value, size_t members)
 {
  Dirt_Struct *res;
  RuneStone_Struct_InstanceStruct *inst;

  if (!type || !class || !name || !value)
   {
    if (class) class->type->decref(session, class);
    if (value)
     for (;members > 0; members--)
      value[members-1]->type->decref(session, value[members-1]);
    return NULL;
   }
  if (!(res = Dirt_Struct_init(session, type)))
   {
    class->type->decref(session, class);
    for (;members > 0; members--)
     value[members-1]->type->decref(session, value[members-1]);
    return NULL;
   }
  inst = (RuneStone_Struct_InstanceStruct *) res;
  inst->class = class;
  inst->name = malloc(sizeof(char *) * members);
  inst->value = malloc(sizeof(Dirt_Struct *) * members);
  inst->members = 0;
  if (!inst->name || !inst->value)
   {
    res->type->decref(session, res);
    for (;members > 0; members--)
     value[members-1]->type->decref(session, value[members-1]);
   }
  memcpy((void *) inst->value, (void *) value, sizeof(Dirt_Struct *) * members);
  for (; inst->members < members; inst->members++)
   {
    if (!(inst->name[inst->members] = malloc(strlen(name[inst->members]) + 1)))
     {
      res->type->decref(session, res);
      return NULL;
     }
    strcpy(inst->name[inst->members], name[inst->members]);
   }
  return res;
 }


Dirt_Struct *RuneStone_Struct_instanceStruct(Dirt_Session *session, Dirt_Struct *class, char **name, Dirt_Struct **value, size_t members)
 {
  return RuneStone_Struct_InstanceStruct_init(session, &RuneStone_StructType_Instance, class, name, value, members);
 }

Dirt_Struct *RuneStone_Struct_builtinBasedInstanceStruct(Dirt_Session *session, Dirt_Struct *class, Dirt_Struct **baseValue, size_t baseValues, char **name, Dirt_Struct **value, size_t members)
 {
  Dirt_Struct *res;
  RuneStone_Struct_BuiltinBasedInstanceStruct *bbinst;

  if (!(res = RuneStone_Struct_InstanceStruct_init(session, &RuneStone_StructType_BuiltinBasedInstance, class, name, value, members)))
   {
    for (; baseValues; baseValues--)
     baseValue[baseValues-1]->type->decref(session, baseValue[baseValues-1]);
    return NULL;
   }
  bbinst = (RuneStone_Struct_BuiltinBasedInstanceStruct *) res;
  bbinst->baseValues = 0;
  bbinst->baseValue = malloc(sizeof(Dirt_Struct *) * baseValues);
  if (!bbinst->baseValue)
   {
    res->type->decref(session, res);
    for (; baseValues; baseValues--)
     baseValue[baseValues-1]->type->decref(session, baseValue[baseValues-1]);
    return NULL;
   }
  memcpy((void *) bbinst->baseValue, (void *) baseValue, sizeof(Dirt_Struct *) * baseValues);
  bbinst->baseValues = baseValues;
  return res;
 }

Dirt_Struct *RuneStone_Struct_typeInstanceStruct(Dirt_Session *session, Dirt_Struct *class, char *instanceName, Dirt_Struct **base, size_t baseClasses, char **name, Dirt_Struct **value, size_t members)
 {
 }
