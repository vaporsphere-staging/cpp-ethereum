/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file QFunctionDefinition.cpp
 * @author Yann yann@ethdev.com
 * @date 2014
 */

#include <libsolidity/AST.h>
#include <libdevcrypto/SHA3.h>
#include "QVariableDeclaration.h"
#include "QFunctionDefinition.h"

using namespace dev::solidity;
using namespace dev::mix;

QFunctionDefinition::QFunctionDefinition(dev::solidity::FunctionDefinition const* _f, int _index): QBasicNodeDefinition(_f), m_index(_index), m_hash(dev::sha3(_f->getCanonicalSignature()))
{
	std::vector<std::shared_ptr<VariableDeclaration>> parameters = _f->getParameterList().getParameters();
	for (unsigned i = 0; i < parameters.size(); i++)
		m_parameters.append(new QVariableDeclaration(parameters.at(i).get()));

	std::vector<std::shared_ptr<VariableDeclaration>> returnParameters = _f->getReturnParameters();
	for (unsigned i = 0; i < returnParameters.size(); i++)
		m_returnParameters.append(new QVariableDeclaration(returnParameters.at(i).get()));
}
