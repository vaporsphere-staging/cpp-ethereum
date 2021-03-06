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
/** @file LogFilter.cpp
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 */

#include "LogFilter.h"

#include <libdevcrypto/SHA3.h>
#include "State.h"
using namespace std;
using namespace dev;
using namespace dev::eth;

void LogFilter::streamRLP(RLPStream& _s) const
{
	_s.appendList(6) << m_addresses << m_topics << m_earliest << m_latest << m_max << m_skip;
}

h256 LogFilter::sha3() const
{
	RLPStream s;
	streamRLP(s);
	return dev::sha3(s.out());
}

bool LogFilter::matches(LogBloom _bloom) const
{
	if (m_addresses.size())
	{
		for (auto i: m_addresses)
			if (_bloom.containsBloom<3>(dev::sha3(i)))
				goto OK1;
		return false;
	}
	OK1:
	if (m_topics.size())
	{
		for (auto i: m_topics)
			if (_bloom.containsBloom<3>(dev::sha3(i)))
				goto OK2;
		return false;
	}
	OK2:
	return true;
}

bool LogFilter::matches(State const& _s, unsigned _i) const
{
	return matches(_s.receipt(_i)).size() > 0;
}

LogEntries LogFilter::matches(TransactionReceipt const& _m) const
{
	LogEntries ret;
	if (matches(_m.bloom()))
		for (LogEntry const& e: _m.log())
		{
			if (!m_addresses.empty() && !m_addresses.count(e.address))
				continue;
			for (auto const& t: m_topics)
				if (!std::count(e.topics.begin(), e.topics.end(), t))
					continue;
			ret.push_back(e);
		}
	return ret;
}
