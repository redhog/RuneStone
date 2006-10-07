#include <RuneStone/Object.h>
#include <RuneStone/Introspection.h>
#include <Dirt/SimpleStruct.h>

void RuneStone_Struct_AnnotatedType_free(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_AnnotatedTypeStruct *strct = (RuneStone_Struct_AnnotatedTypeStruct *) item;
 
  strct->comment->type->free(session, strct->comment);
  strct->value->type->free(session, strct->value);
 }

/* Grimoire.Types.AnnotatedValue('value', 'comment')
   ==>
   <'Object', (<'Type', [['__builtin__', 'object'],
                         ['__builtin__', 'dict'],
			 ['Grimoire', 'Utils', 'Types', 'Maps', 'ImmutableMapping'],
			 ['Grimoire', 'Types', 'Composable', 'Mapping'],
			 ['Grimoire', 'Types', 'Composable', 'AnnotatedValue']]>,
               ([{'comment': 'comment',
	          'value': 'value'}], {}, ), )>

 */
Dirt_Struct *RuneStone_Struct_AnnotatedType_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_AnnotatedTypeStruct *strct = (RuneStone_Struct_AnnotatedTypeStruct *) item;
  Dirst_Struct *res;
  char *cls = 'Grimoire.Types.AnnotatedValue';
  
  res = dict(append(append(strct(),
			   keyval(STR("Value"), strct->value)),
		    keyval(STR("Comment"), strct->comment)));
  res = RuneStone_Struct_instanceStruct(session,
					RuneStone_Struct_classStruct(session, &cls, 1),
					&res,
					1,
					NULL,
					NULL,
					0);
  return res->type->contract(session, res);
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

/* Grimoire.Types.ParamsType.derive([('foo', int), ('bar', str)], 1)
   ==> <'Object', (<'Type', [['Grimoire', 'Types', 'Introspection', 'ParamsType']]>,
		   ('ParamsType',
		    (<'Type', [['Grimoire', 'Types', 'Introspection', 'GenericParamsType']]>, ),
		    {'__module__': 'Grimoire.Types.Derived',
		     '__metaclass__': <'Type', [['Grimoire', 'Types', 'Introspection', 'ParamsType']]>,
		     'required': 1,
		     'tag': ((('foo', <'Type', [['__builtin__', 'int']]>, ),
			      ('bar', <'Type', [['__builtin__', 'str']]>, ), ),
			     1, None, None, None, ),
		     'convertType': None,
		     'reskwtype': None,
		     'arglist': [('foo', <'Type', [['__builtin__', 'int']]>, ),
				 ('bar', <'Type', [['__builtin__', 'str']]>, )],
		     '__doc__': None,
		     'resargstype': None,
		     'argdict': {'foo': <'Type', [['__builtin__', 'int']]>,
				 'bar': <'Type', [['__builtin__', 'str']]>}}, ), )>
*/
Dirt_Struct *RuneStone_Struct_ParamsType_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  paramsTypeClassName = "Grimoire.Types.Introspection.ParamsType";
  genericParamsTypeClassName = "Grimoire.Types.Introspection.GenericParamsType";
  
  cls = RuneStone_Struct_classStruct(session, &paramsTypeClassName, 1)

  RuneStone_Struct_instanceStruct(session, Dirt_Struct *class, Dirt_Struct **baseValue, size_t baseValues, schar **name, Dirt_Struct **value, size_t members)


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


Dirt_StructType RuneStone_StructType_AnnotatedType = { sizeof(RuneStone_Struct_AnnotatedTypeStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_AnnotatedType_free, RuneStone_Struct_AnnotatedType_contract };
Dirt_StructType RuneStone_StructType_ParamsType = { sizeof(RuneStone_Struct_ParamsTypeStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_ParamsType_free, RuneStone_Struct_ParamsType_contract };


Dirt_Struct *RuneStone_Struct_paramsTypeStruct(Dirt_Session *session, char **argname, Dirt_Struct **argtype, size_t args, int required, Dirt_Struct *resargstype, Dirt_Struct *reskwtype,  Dirt_Struct *convertType)
 {
 }
