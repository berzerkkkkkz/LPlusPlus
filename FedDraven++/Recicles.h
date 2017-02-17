#pragma once
#include "PluginSDK.h"
#include <vector>

namespace Draven { class QRecticle; }

namespace Draven
{
	class Recicles
	{

	public:
		std::vector<QRecticle*> getQReticles() const;
		void setQReticles(const std::vector<QRecticle*> &value);

	private:

		class QRecticle
		{
		private:
			int privateExpireTime = 0;
			IUnit* privateObject;


		public:
			QRecticle(IUnit* rectice, int expireTime);

			int getExpireTime() const;
			void setExpireTime(const int &value);

			IUnit* getObject() const;
			void setObject(IUnit* value);

			Vec3 getPosition() const;

		};
	};
}
