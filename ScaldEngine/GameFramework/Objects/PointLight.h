#include "Actor.h"

namespace Scald
{
    class APointLight : public AActor
    {
        using Super = AActor;
    public:
        APointLight();
        virtual ~APointLight();

        virtual void Tick(float DeltaTime) override;
        virtual void OnSpawn(World* owner) override;
        virtual void DestroyActor() override;
    private:
    };
}