#ifndef RENDERER_H
#define RENDERER_H

#include "GameField.h"
#include <string>


class Renderer {
public:
  virtual ~Renderer() = default;
  virtual void render(const GameField *field) = 0;
  virtual void displayMessage(const std::string &message) = 0;
};


class ConsoleRenderer : public Renderer {
private:
  void printHorizontalBorder(int width);
  char getEntitySymbol(int x, int y, const GameField *field);

public:
  void render(const GameField *field) override;
  void displayMessage(const std::string &message) override;
};

#endif 
