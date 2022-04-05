#pragma once

#ifndef MATLANG_PARSER_H
#define MATLANG_PARSER_H

#include <memory>

#include "tokenizer.h"
#include "types/object.h"
#include "types/matrix.h"
#include "types/expression.h"
#include "types/integer.h"

std::list<std::shared_ptr<Object>> ReadScript(Tokenizer *);

std::shared_ptr<Object> Read(Tokenizer *, size_t = 0);

std::list<std::shared_ptr<Object>> ReadExpression(Tokenizer *, bool = false, bool * = nullptr);

std::shared_ptr<Object> ReadCommandArgs(Tokenizer *, std::shared_ptr<Object>, size_t = 0);

std::shared_ptr<Object> ReadMatrix(Tokenizer *);

std::vector<std::shared_ptr<Object>> ReadLine(Tokenizer *);

bool ExpectRead(Tokenizer *, std::string_view);

#endif //MATLANG_PARSER_H
