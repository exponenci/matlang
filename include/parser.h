#pragma once

#include "tokenizer.h"
#include "object.h"
#include "rational.h"
#include "matrix.h"
#include "expression.h"

#include <memory>


std::list<std::shared_ptr<Object>> ReadScript(Tokenizer *);

std::shared_ptr<Object> Read(Tokenizer *, size_t = 0);

std::shared_ptr<Object> ReadExpression(Tokenizer *, bool * = nullptr);

std::shared_ptr<Object> ReadCommandArgs(Tokenizer *, std::shared_ptr<Object>);

std::shared_ptr<Object> ReadMatrix(Tokenizer *);

std::vector<std::shared_ptr<Object>> ReadLine(Tokenizer *);

bool ExpectRead(Tokenizer *, std::string_view);
