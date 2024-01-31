#pragma once

namespace sol {
	class state;
}

namespace Wrapper {

	class LuaWrapper
	{
	public:
		void Bind(sol::state& state);
	};
}
