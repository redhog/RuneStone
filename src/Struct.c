#include <RuneStone/Struct.h>

#define STR(s) Dirt_Struct_str(session, s, sizeof(s) - 1)
#define str(s) Dirt_Struct_str(session, s, strlen(s))
#define blob(s, l) Dirt_Struct_str(session, s, l)
#define keyval(k, v) Dirt_Struct_keyvalue(session, k, v)
#define strct() Dirt_Struct_structure(session)
#define append(st, item) Dirt_Struct_structure_add(session, st, item)
#define type(st) Dirt_Struct_structure_finalize_type(session, st)
#define list(st) Dirt_Struct_structure_finalize_list(session, st)
#define dict(st) Dirt_Struct_structure_finalize_dictionary(session, st)

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
  Dirt_Struct *lst, *pth;
  size_t i, j, k;

  lst = strct();
  for (i = 0; i < strct->paths; i++)
   {
    pth = strct();
    for (j=k=0; strct->path[i][j]; j = ++k)
     {
      for (; strct->path[i][k] && strct->path[i][k] != '.'; k++);
      pth = append(pth, blob(strct->path[i] + j, k - j));
     }
    lst = append(lst, list(pth));
   }
  return type(append(append(strct(),
			    STR("Type")),
		     list(lst)));
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
 * x = X()
 * x.foo = 1
 * ==> <'Object', (<'Type', [['__builtin__', 'object'],
 *                           ['__main__', 'X']]>,
 *                 {'foo': 1})>
 * class X(dict): pass
 * x = X({"bar":2})
 * x.foo = 1
 * ==> <'Object', (<'Type', [['__builtin__', 'object'],
 *                           ['__builtin__', 'dict'],
 *                           ['__main__', 'X']]>,
 *                 ([{'bar': 2}],
 *                  {'foo': 1}))>
 */
Dirt_Struct *RuneStone_Struct_Instance_restrict(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_ClassStruct *strct = (RuneStone_Struct_ClassStruct *) item;
  Dirt_Struct *baseValues;
  Dirt_Struct *members;
  size_t i;

  baseValues = strct();
  for (i = 0; i < strct->baseValues; i++)
   baseValues = append(baseValues,
		       strct->baseValue[i]->type->restrict(session, strct->baseValue[i]));
  baseValues = list(baseValues);
  members = strct();
  for (i = 0; i < strct->members; i++)
   members = append(members,
		    keyval(str(strct->name[i]),
			   strct->value[i]->type->restrict(session, strct->value[i])));
  members = dict(members);

  /* <'Object', (class, (baseValues, members))> */
  return type(append(append(strct(),
			    STR("Object")),
		     list(append(append(strct(),
					strct->class->type->restrict(session, strct->class)),
				 list(append(append(strct(),
						    baseValues),
					     members))))));
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
					dict(append(append(strct(),
							   keyval(STR("Value"), strct->value)),
						    keyval(STR("Comment"), strct->comment))),
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


  return append(append(append(append(append(append(append(append(append(strct(),
									keyval(STR("tag"), )),
								 keyval(STR("reskwtype"), )),
							  keyval(STR("resargstype")), )),
					    keyval(STR("required"), )),
				     keyval(STR("parentType"), )),
			      keyval(STR("convertType"), )),
		       keyval(STR("arglist"), )),
		keyval(STR("argdict"), ));
   

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
