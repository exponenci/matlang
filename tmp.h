#pragma once

#ifndef MATLANG_PARSER_H
#define MATLANG_PARSER_H

#include <memory>

#include "types/object.h"
#include "tokenizer.h"

std::shared_ptr<Object> Read(Tokenizer *, size_t = 0);

std::list<std::shared_ptr<Object>> ReadExpression(Tokenizer *, bool = false, bool * = nullptr);

std::shared_ptr<Object> ReadCommandArgs(Tokenizer *, std::shared_ptr<Object>, size_t = 0);

std::shared_ptr<Object> ReadMatrix(Tokenizer *);

std::vector<std::shared_ptr<Object>> ReadLine(Tokenizer *);

bool ExpectRead(Tokenizer *, std::string_view);

#endif //MATLANG_PARSER_H
