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
/**
 * @author Christian <c@ethdev.com>
 * @author Gav Wood <g@ethdev.com>
 * @date 2014
 * Container of the (implicit and explicit) global objects.
 */

#include <memory>
#include <libsolidity/GlobalContext.h>
#include <libsolidity/AST.h>
#include <libsolidity/Types.h>

using namespace std;

namespace dev
{
namespace solidity
{

GlobalContext::GlobalContext():
m_magicVariables(vector<shared_ptr<MagicVariableDeclaration const>>{make_shared<MagicVariableDeclaration>("block", make_shared<MagicType>(MagicType::Kind::BLOCK)),
					make_shared<MagicVariableDeclaration>("msg", make_shared<MagicType>(MagicType::Kind::MSG)),
					make_shared<MagicVariableDeclaration>("tx", make_shared<MagicType>(MagicType::Kind::TX)),
					make_shared<MagicVariableDeclaration>("suicide",
							make_shared<FunctionType>(strings{"address"}, strings{}, FunctionType::Location::SUICIDE)),
					make_shared<MagicVariableDeclaration>("sha3",
							make_shared<FunctionType>(strings{"hash"}, strings{"hash"}, FunctionType::Location::SHA3)),
					make_shared<MagicVariableDeclaration>("log0",
							make_shared<FunctionType>(strings{"hash"},strings{}, FunctionType::Location::LOG0)),
					make_shared<MagicVariableDeclaration>("log1",
							make_shared<FunctionType>(strings{"hash", "hash"},strings{}, FunctionType::Location::LOG1)),
					make_shared<MagicVariableDeclaration>("log2",
							make_shared<FunctionType>(strings{"hash", "hash", "hash"},strings{}, FunctionType::Location::LOG2)),
					make_shared<MagicVariableDeclaration>("log3",
							make_shared<FunctionType>(strings{"hash", "hash", "hash", "hash"},strings{}, FunctionType::Location::LOG3)),
					make_shared<MagicVariableDeclaration>("log4",
							make_shared<FunctionType>(strings{"hash", "hash", "hash", "hash", "hash"},strings{}, FunctionType::Location::LOG4)),
					make_shared<MagicVariableDeclaration>("sha256",
							make_shared<FunctionType>(strings{"hash"}, strings{"hash"}, FunctionType::Location::SHA256)),
					make_shared<MagicVariableDeclaration>("ecrecover",
							make_shared<FunctionType>(strings{"hash", "hash8", "hash", "hash"}, strings{"address"}, FunctionType::Location::ECRECOVER)),
					make_shared<MagicVariableDeclaration>("ripemd160",
							make_shared<FunctionType>(strings{"hash"}, strings{"hash160"}, FunctionType::Location::RIPEMD160))})
{
}

void GlobalContext::setCurrentContract(ContractDefinition const& _contract)
{
	m_currentContract = &_contract;
}

vector<Declaration const*> GlobalContext::getDeclarations() const
{
	vector<Declaration const*> declarations;
	declarations.reserve(m_magicVariables.size());
	for (ASTPointer<Declaration const> const& variable: m_magicVariables)
		declarations.push_back(variable.get());
	return declarations;
}

MagicVariableDeclaration const* GlobalContext::getCurrentThis() const
{
	if (!m_thisPointer[m_currentContract])
		m_thisPointer[m_currentContract] = make_shared<MagicVariableDeclaration>(
													"this", make_shared<ContractType>(*m_currentContract));
	return m_thisPointer[m_currentContract].get();

}

}
}
