#ifndef PHYSICS_SIMULATOR_H
#define PHYSICS_SIMULATOR_H

#include "rigid_body.h"
#include <GXCommon/gx_memory.h>
#include <QVector>

namespace gx_engine {

struct Contact;
class Simplex;
struct SupportPoint;
struct Edge;
struct Triangle;
class PhysicsSimulator {
public:
    ~PhysicsSimulator();

    void start();
    void stop();

    void addRigidBody(RigidBody& rigidBody);
    void removeRigidBody(RigidBody& rigidBody);

    void setSimulationTimeStep(float step);         // sec
    float getSimutalionTimeStep() const;

    void simulate(float deltaTime);                 // sec

    bool runGJK(const PhysicsShapeLink& shapeA, const PhysicsShapeLink& shapeB);

    static PhysicsSimulator* instance();

private:
    PhysicsSimulator();

    void collectContacts();
    void resolveContacts();

    void support(SupportPoint& point, const PhysicsShape& shapeA, const gx_common::GXMat4& transformA,
                 const PhysicsShape& shapeB, const gx_common::GXMat4& transformB, const gx_common::GXVec3& direction);

    void modifySimplex(Simplex& simplex, gx_common::GXVec3& direction);
    void addEdgeWithChecking(QVector<Edge>& edges, const SupportPoint& edgeStart, const SupportPoint& edgeEnd);
    bool simpleWindingCheck(const Triangle& triangle, const gx_common::GXVec3 & point);
    bool getContact(const Triangle& triangle, const PhysicsShapeLink& shapeA, const PhysicsShapeLink& shapeB);

    static PhysicsSimulator* _instance;
    float _simulationTimeStep;
    QVector<RigidBody*> _rigidBodies;
    gx_common::GXCircleBuffer _buffer;
    Contact* _contacts;
};

} // namespace gx_engine

#endif PHYSICS_SIMULATOR_H
