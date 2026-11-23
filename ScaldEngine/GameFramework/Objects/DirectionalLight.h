#include "Actor.h"

namespace Scald
{
    class ADirectionalLight : public AActor
    {
        using Super = AActor;

    public:
        ADirectionalLight();
        virtual ~ADirectionalLight();

        virtual void Tick(float DeltaTime) override;
        virtual void OnSpawn(World* owner) override;
        virtual void DestroyActor() override;
    private:

    };
}