#ifndef GUN_H
#define GUN_H_
namespace game {
    namespace weapons {

        class Gun {
        public:
            Gun();

            virtual ~Gun();

            virtual void Render(Camera &camera) = 0;

            virtual void Update(float deltaTime) = 0;

            virtual bool CanFire() = 0;

            virtual void Fire(Camera &camera) = 0;

        protected:
            Mesh *mMesh;
        };
    }
}

#endif
