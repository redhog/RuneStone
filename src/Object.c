#include <RuneStone/Object.h>
#include <Dirt/SimpleStruct.h>

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
  RuneStone_Struct_ClassStruct *strct = (RuneStone_Struct_ClassStruct *) item;
  size_t path;

  for (path = 0; path < strct->paths; path++)
   free(strct->path[path]);
  free(strct->path);
 }

/* Grimoire.Types.AnnotatedValue
   ==> <'Type', [['__builtin__', 'object'],
  	         ['Grimoire', 'Types', 'Composable', 'Composable'],
  		 ['Grimoire', 'Types', 'Composable', 'Mapping'],
  		 ['Grimoire', 'Types', 'Composable', 'AnnotatedValue']]>
 */
Dirt_Struct *RuneStone_Struct_Class_contract(Dirt_Session *session, Dirt_Struct *item)
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
  /
Dirt_Struct *RuneStone_Struct_Instance_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  RuneStone_Struct_ClassStruct *strct = (RuneStone_Struct_ClassStruct *) item;
  Dirt_Struct *baseValues;
  Dirt_Struct *members;
  Dirt_Struct *content;
  size_t i;

  members = strct();
  for (i = 0; i < strct->members; i++)
   members = append(members,
		    keyval(str(strct->name[i]),
			   strct->value[i]->type->contract(session, strct->value[i])));
  members = dict(members);

  if (strct->baseValues)
   {
    baseValues = strct();
    for (i = 0; i < strct->baseValues; i++)
     baseValues = append(baseValues,
			 strct->baseValue[i]->type->contract(session, strct->baseValue[i]));
    baseValues = list(baseValues);
    content = list(append(append(strct(),
				 baseValues),
			  members));
   }
  else
   content = members;

  return type(append(append(strct(),
			    STR("Object")),
		     list(append(append(strct(),
					strct->class->type->contract(session, strct->class)),
				 content))));
 }


Dirt_StructType RuneStone_StructType_Class = { sizeof(RuneStone_Struct_ClassStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_Class_free, RuneStone_Struct_Class_contract };;
Dirt_StructType RuneStone_StructType_Instance = { sizeof(RuneStone_Struct_InstanceStruct), Dirt_Struct_decref, Dirt_Struct_incref, RuneStone_Struct_Instance_free, RuneStone_Struct_Instance_contract };

Dirt_Struct *RuneStone_Struct_classStruct(Dirt_Session *session, char **path, size_t paths)
 {
 }

Dirt_Struct *RuneStone_Struct_instanceStruct(Dirt_Session *session, Dirt_Struct *class, Dirt_Struct **baseValue, size_t baseValues, schar **name, Dirt_Struct **value, size_t members)
 {
 }
