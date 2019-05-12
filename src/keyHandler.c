#include "keyHandler.h"

bool key_2nd[2];
bool key_Del[2];

bool key_up[2];
bool key_down[2];
bool key_left[2];
bool key_right[2];

void setKey() {
  kb_Scan();

  key_2nd[0] = kb_Data[1] == kb_2nd;
  key_Del[0] = kb_Data[1] == kb_Del;
  key_up[0] = kb_Data[7] == kb_Up;
  key_down[0] = kb_Data[7] == kb_Down;
  key_left[0] = kb_Data[7] == kb_Left;
  key_right[0] = kb_Data[7] == kb_Right;
}

void setPreKey() {
  key_2nd[1] = key_2nd[0];
  key_Del[1] = key_Del[0];
  key_up[1] = key_up[0];
  key_down[1] = key_down[0];
  key_left[1] = key_left[0];
  key_right[1] = key_right[0];
}

bool key(bool keys[2]) {
  return keys[0] && ! keys[1];
}