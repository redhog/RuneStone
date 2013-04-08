#include <Dirt/Writer.h>
#include <Dirt/Struct.h>
#include <Dirt/SimpleStruct.h>
#include <Dirt/StructWriter.h>
#include <Dirt/WriteBufferTypes.h>
#include <RuneStone/Object.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

int main(int argc, char **argv)
 {
  Dirt_Session *session = &Dirt_DebugSession;
  Dirt_FdWriteBuffer fdbuffer;
  Dirt_WriteBuffer *buffer = (Dirt_WriteBuffer *) &fdbuffer;
  Dirt_Writer writer;
  Dirt_Struct *lst, *clst;
  char *path = "foo.bar.fie";
  int fd;

  if ((fd = open("ObjectTest.output", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) < 0)
   {
    perror("Unable to open file");
    exit(1);
   }
  if (!Dirt_FdWriteBuffer_init(&fdbuffer, &Dirt_DebugSession, fd))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize FdWriteBuffer");
    exit(1);
   }
  if (!Dirt_StandardWriter_init(&writer, &Dirt_StructWriter_Callbacks, buffer))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize Writer");
    printf("Unable to initialize Writer");
    exit(1);
   }

  lst = list(append(strct(),
		  RuneStone_Struct_classStruct(session, &path, 1)));
  if (!(clst = lst->type->contract(session, lst)))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Data error", "Unable to cvontract data");
    exit(1);
   }
  if (!writer.callback->write(&writer, clst))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to write data");
    exit(1);
   }
  clst->type->decref(&Dirt_DebugSession, clst);
  lst->type->decref(&Dirt_DebugSession, lst);
  buffer->type->free(buffer);
  exit(0);
 }
