#include <RuneStone/Struct.h>

/* To test how some data *should* be serialized, check using the
 * following Python-code:
 *
 * import Grimoire
 * Grimoire.Utils.Serialize.Writer.contract(
 *     somedata,
 *     Grimoire.Utils.Serialize.RPC.ObjectMappedExtension(
 *         Grimoire.Utils.Serialize.RPC.stdObjectMap()).serialize)
 */

void RuneStone_Struct_Class_free(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_ClassStruct *strct = (RuneStone_Struct_ClassStruct *) item;
  size_t path;

  for (path = 0; path < strct->paths; path++)
   free(strct->path[path]);
  free(strct->path);
 }

/* Grimoire.Types.AnnotatedValue
 * ==> <'Type', [['__builtin__', 'object'],
 *	         ['Grimoire', 'Types', 'Composable', 'Composable'],
 *		 ['Grimoire', 'Types', 'Composable', 'Mapping'],
 *		 ['Grimoire', 'Types', 'Composable', 'AnnotatedValue']]>
 */
Dirt_Struct *RuneStone_Struct_Class_restrict(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_ClassStruct *strct = (RuneStone_Struct_ClassStruct *) item;
  Dirt_Struct *res, *lst, *pth;
  size_t i, j, k;

  res = Dirt_Struct_structure_add(session,
				  Dirt_Struct_structure(session),
				  Dirt_Struct_str(session, "Type", sizeof("Type") - 1));

  lst = Dirt_Struct_structure(session);
  for (i = 0; i < strct->paths; i++)
   {
    pth = Dirt_Struct_structure(session);
    for (j=k=0; strct->path[i][j]; j = ++k)
     {
      for (; strct->path[i][k] && strct->path[i][k] != '.'; k++);
      pth = Dirt_Struct_structure_add(session,
				      pth,
				      Dirt_Struct_str(session, strct->path[i] + j, k - j));
     }
    lst = Dirt_Struct_structure_add(session,
				    lst,
				    Dirt_Struct_structure_finalize_list(session, pth));
   }
  res = Dirt_Struct_structure_add(session,
				  res,
				  Dirt_Struct_structure_finalize_list(session, lst));

  return Dirt_Struct_structure_finalize_type(session, res);
 }

void RuneStone_Struct_Instance_free(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_InstanceStruct *strct = (RuneStone_Struct_InstanceStruct *) item;
  size_t member;
  
  strct->class->type->decref(session, strct->class);
  for (member = 0; member < strct->baseValues; member++)
   strct->baseValue[member]->type->decref(session, strct->baseValue[member]);
  free(strct->baseValue);
  for (member = 0; member < strct->members; member++)
   {
    free(strct->name[member]);
    strct->value[member]->type->decref(session, strct->value[member]);
   }
  free(strct->name);
  free(strct->value);
 }

/* class X(object): pass
   x = X()
   x.foo = 1
   ==> <'Object', (<'Type', [['__builtin__', 'object'],
                             ['__main__', 'X']]>,
                   {'foo': 1})>
   class X(dict): pass
   x = X({"bar":2})
   x.foo = 1
   ==> <'Object', (<'Type', [['__builtin__', 'object'],
                             ['__builtin__', 'dict'],
                             ['__main__', 'X']]>,
                   ([{'bar': 2}],
                    {'foo': 1}))>
>

 */
Dirt_Struct *RuneStone_Struct_Instance_restrict(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_ClassStruct *strct = (RuneStone_Struct_ClassStruct *) item;
  Dirt_Struct *baseValues;
  Dirt_Struct *members;
  size_t i;

  baseValues = Dirt_Struct_structure(session);
  for (i = 0; i < strct->baseValues; i++)
   baseValues = Dirt_Struct_structure_add(session, baseValues,
					  strct->baseValue[i]->type->restrict(session, strct->baseValue[i]));
  baseValues = Dirt_Struct_structure_finalize_list(baseValues);
  members = Dirt_Struct_structure(session);
  for (i = 0; i < strct->members; i++)
   members = Dirt_Struct_structure_add(session, members,
				       Dirt_Struct_keyvalue(session,
							    Dirt_Struct_str(session, strct->name[i], strlen(strct->name[i])),
							    strct->value[i]->type->restrict(session, strct->value[i])));
  members = Dirt_Struct_structure_finalize_dictionary(members);

  /* <'Object', (class, (baseValues, members))> */
  return Dirt_Struct_structure_finalize_type(session,
   Dirt_Struct_structure_add(session,
    Dirt_Struct_structure_add(session,
     Dirt_Struct_structure(session),
     Dirt_Struct_str(session, "Object", sizeof("Object") - 1)),
    Dirt_Struct_structure_finalize_list(
     Dirt_Struct_structure_add(session,
      Dirt_Struct_structure_add(session,
       Dirt_Struct_structure(session),
       strct->class->type->restrict(session, strct->class)),
      Dirt_Struct_structure_finalize_list(
       Dirt_Struct_structure_add(session,
        Dirt_Struct_structure_add(session,
         Dirt_Struct_structure(session),
	 baseValues),
        members))))))	 
 }

void RuneStone_Struct_AnnotatedType_free(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_AnnotatedTypeStruct *strct = (RuneStone_Struct_AnnotatedTypeStruct *) item;
 
  strct->comment->type->free(session, strct->comment);
  strct->value->type->free(session, strct->value);
 }

Dirt_Struct *RuneStone_Struct_AnnotatedType_restrict(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_AnnotatedTypeStruct *strct = (RuneStone_Struct_AnnotatedTypeStruct *) item;
  Dirst_Struct *res;
  char *cls = 'Grimoire.Types.AnnotatedValue';

  res = RuneStone_Struct_instanceStruct(session,
   RuneStone_Struct_classStruct(session, &cls, 1),
   Dirt_Struct_structure_finalize_dictionary(
    Dirt_Struct_structure_add(session,
     Dirt_Struct_structure_add(session,
      Dirt_Struct_structure(session),
      Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "Value", sizeof("Value") - 1)), strct->value)),
     Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "Comment", sizeof("Comment") - 1)), strct->comment))),
   1,
   NULL,
   NULL,
   0);
  return res->type->restrict(session, res);
 }

void RuneStone_Struct_ParamsType_free(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_ParamsTypeStruct *strct = (RuneStone_Struct_ParamsTypeStruct *) item;
  size_t arg;

  for (arg = 0; arg < strct->args; arg++)
   {
    free(strct->argname[arg]);
    strct->argtype[arg]->type->decref(session, strct->argtype[arg]);
   }
  free(strct->argname);
  free(strct->argtype);
  strct->resargstype->type->decref(session, strct->resargstype);
  strct->reskwtype->type->decref(session, strct->reskwtype);
  strct->reskwtype->type->decref(session, strct->reskwtype);
  strct->convertType->type->decref(session, strct->convertType);
 }

Dirt_Struct *RuneStone_Struct_ParamsType_restrict(Dirt_Session *session, Dirt_Struct *item)
 {

"Grimoire.Types.ParamsType"

'argdict', 'arglist', 'convertType', 'parentType', 'required', 'resargstype', 'reskwtype', 'tag'

  Dirt_Struct_structure_add(session,
   Dirt_Struct_structure_add(session,
    Dirt_Struct_structure_add(session,
     Dirt_Struct_structure_add(session,
      Dirt_Struct_structure_add(session,
       Dirt_Struct_structure_add(session,
        Dirt_Struct_structure_add(session,
	 Dirt_Struct_structure_add(session,
	  Dirt_Struct_structure_add(session,
	   Dirt_Struct_structure(session),
	   Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "tag", sizeof("tag") - 1), )),
	  Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "reskwtype", sizeof("reskwtype") - 1), )),
	 Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "resargstype", sizeof("resargstype") - 1)), )),
	Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "required", sizeof("required") - 1), )),
       Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "parentType", sizeof("parentType") - 1), )),
      Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "convertType", sizeof("convertType") - 1), )),
     Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "arglist", sizeof("arglist") - 1), )),
    Dirt_Struct_keyvalue(session, Dirt_Struct_str(session, "argdict", sizeof("argdict") - 1), ))


 }

Dirt_StructType RuneStone_StructType_Class = { sizeof(RuneStone_Struct_ClassStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_Class_free, RuneStone_Struct_Class_restrict };;
Dirt_StructType RuneStone_StructType_Instance = { sizeof(RuneStone_Struct_InstanceStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_Instance_free, RuneStone_Struct_Instance_restrict };
Dirt_StructType RuneStone_StructType_AnnotatedType = { sizeof(RuneStone_Struct_AnnotatedTypeStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_AnnotatedType_free, RuneStone_Struct_AnnotatedType_restrict };
Dirt_StructType RuneStone_StructType_ParamsType = { sizeof(RuneStone_Struct_ParamsTypeStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_ParamsType_free, RuneStone_Struct_ParamsType_restrict };

Dirt_Struct *RuneStone_Struct_classStruct(Dirt_Session *session, char **path, size_t paths)
 {
 }

Dirt_Struct *RuneStone_Struct_instanceStruct(Dirt_Session *session, Dirt_Struct *class, Dirt_Struct **baseValue, size_t baseValue, schar **name, Dirt_Struct **value, size_t members)
 {
 }

Dirt_Struct *RuneStone_Struct_paramsTypeStruct(Dirt_Session *session, char **argname, Dirt_Struct **argtype, size_t args, int required, Dirt_Struct *resargstype, Dirt_Struct *reskwtype,  Dirt_Struct *convertType)
 {
 }
