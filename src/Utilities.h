#pragma once

static int currentId = 0;

int getNewAkGameObjectId()
{
    currentId++;
    return currentId;
}