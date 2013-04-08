#ifndef RUNE_STONE_OBJECT_H
#define RUNE_STONE_OBJECT_H

#include <Dirt/Struct.h>


typedef struct RuneStone_Struct_ClassStructT RuneStone_Struct_ClassStruct;
Dirt_StructType RuneStone_StructType_Class;
struct RuneStone_Struct_ClassStructT {
 Dirt_Struct base;
 char **path; /* Array of strings like "Grimoire.Types.ParamsType" */
 size_t paths;
};

typedef struct RuneStone_Struct_InstanceStructT RuneStone_Struct_InstanceStruct;
Dirt_StructType RuneStone_StructType_Instance;
struct RuneStone_Struct_InstanceStructT {
 Dirt_Struct base;
 Dirt_Struct *class;
 char **name;
 Dirt_Struct **value;
 size_t members;
};

typedef struct RuneStone_Struct_BuiltinBasedInstanceStructT RuneStone_Struct_BuiltinBasedInstanceStruct;
Dirt_StructType RuneStone_StructType_BuiltinBasedInstance;
struct RuneStone_Struct_BuiltinBasedInstanceStructT {
 RuneStone_Struct_InstanceStruct base;
 Dirt_Struct **baseValue;
 size_t baseValues;
};

typedef struct RuneStone_Struct_TypeInstanceStructT RuneStone_Struct_TypeInstanceStruct;
Dirt_StructType RuneStone_StructType_TypeInstance;
struct RuneStone_Struct_TypeInstanceStructT {
 RuneStone_Struct_InstanceStruct base;
 char *name;
 Dirt_Struct **baseClass;
 size_t baseClasses;
};

Dirt_Struct *RuneStone_Struct_classStruct(Dirt_Session *session, char **path, size_t paths);
Dirt_Struct *RuneStone_Struct_instanceStruct(Dirt_Session *session, Dirt_Struct *class, char **name, Dirt_Struct **value, size_t members);
Dirt_Struct *RuneStone_Struct_builtinBasedInstanceStruct(Dirt_Session *session, Dirt_Struct *class, Dirt_Struct **baseValue, size_t baseValues, char **name, Dirt_Struct **value, size_t members);
Dirt_Struct *RuneStone_Struct_typeInstanceStruct(Dirt_Session *session, Dirt_Struct *class, char *instanceName, Dirt_Struct **baseClass, size_t baseClasses, char **name, Dirt_Struct **value, size_t members);

#endif
