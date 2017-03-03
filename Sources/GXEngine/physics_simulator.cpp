#include <GXEngine/physics_simulator.h>
#include <GXCommon/gx_log.h>

#define DEFAULT_SIMULATION_TIME_STEP 0.01665f
#define DEFAULT_SIMULATION_BUFFER_SIZE 134217728 // 128 Mb
#define DEFAULT_MINKOWSKI_EPSILON 0.001f
#define DEFAULT_MAX_EPA_ITERATIONS 35
#define DEFAULT_MIN_EPA_DOT_DISTANCE 0.001f
#define DEFAULT_EDGE_EPSILON 1.0e-3f

namespace gx_engine {

struct Contact {
    Contact* next;

    RigidBody* a;
    RigidBody* b;
    gx_common::GXVec3 normal;
    gx_common::GXVec3 location;
    float penetration;

    void print();
};

void Contact::print()
{
    gx_common::gxLogA("====Contact====\n");
    gx_common::gxLogA("Normal: %f %f %f\n", normal.x, normal.y, normal.z);
    gx_common::gxLogA("Location: %f %f %f\n", location.x, location.y, location.z);
    gx_common::gxLogA("Depth: %f\n\n", penetration);
}

//----------------------------------------------------------------------------

struct SupportPoint {
    gx_common::GXVec3 difference;

    gx_common::GXVec3 extremePointA;
    gx_common::GXVec3 extremePointB;
};

class Simplex {
public:
    Simplex();
    ~Simplex();

    void addPoint(const SupportPoint& point);
    void reset();
    unsigned char getNumPoints() const;

    SupportPoint getA();
    SupportPoint getB();
    SupportPoint getC();
    SupportPoint getD();

    bool dotTestWithAO(const gx_common::GXVec3& v);

private:
    QVector<SupportPoint> _points;
    unsigned char _numPoints;
};

Simplex::Simplex()
    : _numPoints(0)
{
    // NOTHING
}

Simplex::~Simplex()
{
    // NOTHING
}

void Simplex::addPoint(const SupportPoint& point)
{
    _numPoints++;
    _points.push_front(point);
}

void Simplex::reset()
{
    _numPoints = 0;
    _points.clear();
}

unsigned char Simplex::getNumPoints() const
{
    return _numPoints;
}

SupportPoint Simplex::getA()
{
    return _points[0];
}

SupportPoint Simplex::getB()
{
    return _points[1];
}

SupportPoint Simplex::getC()
{
    return _points[2];
}

SupportPoint Simplex::getD()
{
    return _points[3];
}

bool Simplex::dotTestWithAO(const gx_common::GXVec3& v)
{
    gx_common::GXVec3 ao(-_points[0].difference.x, -_points[0].difference.y, -_points[0].difference.z);
    return gx_common::gxDotVec3Fast(v, ao) > 0;
}

//----------------------------------------------------------------------------

struct Triangle {
    SupportPoint points[3];
    gx_common::GXVec3 normal;

    Triangle();
    Triangle(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c);

    void init(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c);
};

Triangle::Triangle()
{
    memset(points, 0, sizeof(SupportPoint) * 3);
    memset(&normal, 0, sizeof(gx_common::GXVec3));
}

Triangle::Triangle(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c)
{
    init(a, b, c);
}

void Triangle::init(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c)
{
    points[0] = a;
    points[1] = b;
    points[2] = c;

    gx_common::GXVec3 ab;
    gx_common::GXVec3 ac;
    gx_common::gxSubVec3Vec3(ab, b.difference, a.difference);
    gx_common::gxSubVec3Vec3(ac, c.difference, a.difference);
    gx_common::gxCrossVec3Vec3(normal, ab, ac);
    gx_common::gxNormalizeVec3(normal);
}

//----------------------------------------------------------------------------

struct Edge {
    SupportPoint points[2];

    Edge();
    Edge(const SupportPoint& a, const SupportPoint& b);
};

Edge::Edge()
{
    memset(points, 0, sizeof(SupportPoint) * 2);
}

Edge::Edge(const SupportPoint& a, const SupportPoint& b)
{
    points[0] = a;
    points[1] = b;
}

//----------------------------------------------------------------------------

PhysicsSimulator* PhysicsSimulator::_instance = nullptr;

PhysicsSimulator::~PhysicsSimulator()
{
    _instance = nullptr;
}

void PhysicsSimulator::start()
{
    // TODO
}

void PhysicsSimulator::stop()
{
    // TODO
}

void PhysicsSimulator::addRigidBody(RigidBody& rigidBody)
{
    if (_rigidBodies.indexOf(&rigidBody) != -1) return;

    _rigidBodies.push_back(&rigidBody);
}

void PhysicsSimulator::removeRigidBody(RigidBody& rigidBody)
{
    int i = _rigidBodies.indexOf(&rigidBody);
    if (i == -1) return;

    _rigidBodies.remove(i);
}

void PhysicsSimulator::setSimulationTimeStep(float step)
{
    _simulationTimeStep = step;
}

float PhysicsSimulator::getSimutalionTimeStep() const
{
    return _simulationTimeStep;
}

void PhysicsSimulator::simulate(float /*deltaTime*/)
{
    // TODO
}

bool PhysicsSimulator::runGJK(const PhysicsShapeLink& shapeA, const PhysicsShapeLink& shapeB)
{
    Simplex simplex;
    gx_common::GXVec3 direction(1.0f, 0.0f, 0.0f);

    SupportPoint a;

    gx_common::GXMat4 transformA;
    transformA.from(shapeA.shapeRotation, shapeA.shapePivotOffset);
    gx_common::GXMat4 rigidBodyTransformA;
    rigidBodyTransformA.from(shapeA.rigidBody->getRotation(), shapeA.rigidBody->getLocationWorld());

    gx_common::GXMat4 worldTransformA;
    gx_common::gxMulMat4Mat4(worldTransformA, transformA, rigidBodyTransformA);

    gx_common::GXMat4 transformB;
    transformB.from(shapeB.shapeRotation, shapeB.shapePivotOffset);
    gx_common::GXMat4 rigidBodyTransformB;
    rigidBodyTransformB.from(shapeB.rigidBody->getRotation(), shapeB.rigidBody->getLocationWorld());

    gx_common::GXMat4 worldTransformB;
    gx_common::gxMulMat4Mat4(worldTransformB, transformB, rigidBodyTransformB);

    support(a, *shapeA.shape, worldTransformA, *shapeB.shape, worldTransformB, direction);
    simplex.addPoint(a);

    direction = gx_common::gxCreateVec3(-a.difference.x, -a.difference.y, -a.difference.z);

    bool isIntersected = false;
    bool isLoop = true;

    while (isLoop) {
        support(a, *shapeA.shape, worldTransformA, *shapeB.shape, worldTransformB, direction);
        if (gx_common::gxDotVec3Fast(a.difference, direction) < 0) {
            isLoop = false;
            isIntersected = false;
            continue;
        }

        simplex.addPoint(a);

        switch (simplex.getNumPoints()) {
            case 2: {
                SupportPoint a = simplex.getA();
                SupportPoint b = simplex.getB();
                gx_common::GXVec3 ab;
                gx_common::gxSubVec3Vec3(ab, b.difference, a.difference);

                gx_common::GXVec3 crossABxAO;
                gx_common::gxCrossVec3Vec3(crossABxAO, ab, gx_common::gxCreateVec3(-a.difference.x, -a.difference.y,
                                                                                   -a.difference.z));
                gx_common::gxCrossVec3Vec3(direction, crossABxAO, ab);
                break;
            }
            case 3: {
                SupportPoint a = simplex.getA();
                SupportPoint b = simplex.getB();
                SupportPoint c = simplex.getC();

                gx_common::GXVec3 ab;
                gx_common::GXVec3 ac;
                gx_common::GXVec3 crossABxAC;

                gx_common::gxSubVec3Vec3(ab, b.difference, a.difference);
                gx_common::gxSubVec3Vec3(ac, c.difference, a.difference);
                gx_common::gxCrossVec3Vec3(crossABxAC, ab, ac);

                gx_common::GXVec3 v;
                gx_common::gxCrossVec3Vec3(v, ab, crossABxAC);
                if (simplex.dotTestWithAO(v)) {
                    // origin is outside the triangle, near the edge ab
                    // reset the simplex to the line ab and continue
                    // search direction is perpendicular to ab and parallel to ao

                    simplex.reset();
                    simplex.addPoint(b);
                    simplex.addPoint(a);

                    gx_common::GXVec3 crossABxAO;
                    gx_common::gxCrossVec3Vec3(crossABxAO, ab, gx_common::gxCreateVec3(-a.difference.x, -a.difference.y,
                                                                                       -a.difference.z));
                    gx_common::gxCrossVec3Vec3(direction, crossABxAO, ab);

                    continue;
                }

                gx_common::gxCrossVec3Vec3(v, crossABxAC, ac);
                if (simplex.dotTestWithAO(v)) {
                    // origin is outside the triangle, near the edge ac
                    // reset the simplex to the line ac and continue
                    // search direction is perpendicular to ac and parallel to ao

                    simplex.reset();
                    simplex.addPoint(c);
                    simplex.addPoint(a);

                    gx_common::GXVec3 crossACxAO;
                    gx_common::gxCrossVec3Vec3(crossACxAO, ac, gx_common::gxCreateVec3(-a.difference.x, -a.difference.y,
                                                                                       -a.difference.z));
                    gx_common::gxCrossVec3Vec3(direction, crossACxAO, ac);

                    continue;
                }

                // origin is within the triangular prism defined by the triangle
                // determine if it is above or below
                if (simplex.dotTestWithAO(crossABxAC)) {
                    // origin is above the triangle, so the simplex is not modified,
                    // the search direction is the triangle's face normal

                    direction = crossABxAC;
                    continue;
                }

                // origin is below the triangle, so the simplex is rewound the oposite direction
                // the search direction is the new triangle's face normal
                simplex.reset();
                simplex.addPoint(b);
                simplex.addPoint(c);
                simplex.addPoint(a);

                direction = gx_common::gxCreateVec3(-crossABxAC.x, -crossABxAC.y, -crossABxAC.z);
                break;
            }
            case 4: {
                SupportPoint a = simplex.getA();
                SupportPoint b = simplex.getB();
                SupportPoint c = simplex.getC();
                SupportPoint d = simplex.getD();

                gx_common::GXVec3 ab;
                gx_common::gxSubVec3Vec3(ab, b.difference, a.difference);

                gx_common::GXVec3 ac;
                gx_common::gxSubVec3Vec3(ac, c.difference, a.difference);

                gx_common::GXVec3 cross;
                gx_common::gxCrossVec3Vec3(cross, ab, ac);
                if (simplex.dotTestWithAO(cross)) {
                    modifySimplex(simplex, direction);
                    continue;
                }

                gx_common::GXVec3 ad;
                gx_common::gxSubVec3Vec3(ad, d.difference, a.difference);
                gx_common::gxCrossVec3Vec3(cross, ac, ad);
                if (simplex.dotTestWithAO(cross)) {
                    simplex.reset();
                    simplex.addPoint(d);
                    simplex.addPoint(c);
                    simplex.addPoint(a);

                    modifySimplex(simplex, direction);
                    continue;
                }

                gx_common::gxCrossVec3Vec3(cross, ad, ab);
                if (simplex.dotTestWithAO(cross)) {
                    simplex.reset();
                    simplex.addPoint(b);
                    simplex.addPoint(d);
                    simplex.addPoint(a);

                    modifySimplex(simplex, direction);
                    continue;
                }

                isIntersected = true;
                isLoop = false;
            }
        }
    }

    if (!isIntersected) return false;

    QVector<Triangle> triangles;
    QVector<Edge> edges;

    // Building the initial polytope
    Triangle triangle(simplex.getA(), simplex.getB(), simplex.getC());
    triangles.push_back(triangle);

    triangle.init(simplex.getA(), simplex.getC(), simplex.getD());
    triangles.push_back(triangle);

    triangle.init(simplex.getA(), simplex.getD(), simplex.getB());
    triangles.push_back(triangle);

    triangle.init(simplex.getB(), simplex.getD(), simplex.getC());
    triangles.push_back(triangle);

    int iteration = 0;

    while (true) {
        iteration++;

        if (iteration > DEFAULT_MAX_EPA_ITERATIONS) {
            gx_common::gxLogA("EPA max iterations - ");
            return true;
        }

        float minDotDistance = FLT_MAX;
        unsigned char nearFace = 0;

        for (unsigned char i = 0; i < 4; i++) {
            Triangle triangle = triangles[i];
            float dotDistance = gx_common::gxDotVec3Fast(triangle.normal, triangle.points[0].difference);
            if (dotDistance < minDotDistance) {
                nearFace = i;
                minDotDistance = dotDistance;
            }
        }

        // Now we know that triangle with index "nearFace" is the closest to the origin
        Triangle closestTriangle = triangles[nearFace];

        // Find support point in normal direction of the closest triangle
        SupportPoint a;
        support(a, *shapeA.shape, worldTransformA, *shapeB.shape, worldTransformB, closestTriangle.normal);

        float newDotDistance = gx_common::gxDotVec3Fast(closestTriangle.normal, a.difference);
        float difference = newDotDistance - minDotDistance;

        if (difference < DEFAULT_MIN_EPA_DOT_DISTANCE) {
            gx_common::gxLogA("EPA: exit growth: iteration %i, distance %f\n", iteration, difference);
            getContact(closestTriangle, shapeA, shapeB);
            return true;
        }

        QVector<int> trianglesToDelete;

        int numTriangles = triangles.size();
        for (int i = 0; i < numTriangles; i++) {
            Triangle triangle = triangles[i];
            if (simpleWindingCheck(triangle, a.difference)) {
                addEdgeWithChecking(edges, triangle.points[0], triangle.points[1]);
                addEdgeWithChecking(edges, triangle.points[1], triangle.points[2]);
                addEdgeWithChecking(edges, triangle.points[2], triangle.points[0]);

                trianglesToDelete.push_back(i);
            }
        }

        int numTrianglesToDelete = trianglesToDelete.size();
        for (int i = 0; i < numTrianglesToDelete; i++) {
            triangles.remove(trianglesToDelete[i] - i);
        }

        int numEdges = edges.size();
        for (int i = 0; i < numEdges; i++) {
            Edge edge = edges[i];
            triangles.push_back(Triangle(a, edge.points[0], edge.points[1]));
        }

        edges.clear();
    }

    return isIntersected;
}

PhysicsSimulator* PhysicsSimulator::instance()
{
    if (!_instance) {
        _instance = new PhysicsSimulator();
    }

    return _instance;
}

PhysicsSimulator::PhysicsSimulator()
    : _buffer(DEFAULT_SIMULATION_BUFFER_SIZE)
    , _contacts(nullptr)
{
    setSimulationTimeStep(DEFAULT_SIMULATION_TIME_STEP);
}

void PhysicsSimulator::collectContacts()
{
    int totalBodies = _rigidBodies.size();

    if (totalBodies < 1) return;

    int limit = totalBodies - 1;
    for (int i = 0; i < limit; i++) {
        for (int j = i + 1; j < totalBodies; j++) {
            if (!gx_common::gxIsOverlapedAABBAABB(_rigidBodies[i]->getBoundsWorld(),
                                                 _rigidBodies[j]->getBoundsWorld())) {
                continue;
            }

            PhysicsShapeLink* shapesA = _rigidBodies[i]->getShapes();
            PhysicsShapeLink* shapesB = _rigidBodies[j]->getShapes();

            for (PhysicsShapeLink* x = shapesA; x; x = x->next) {
                for (PhysicsShapeLink* y = shapesB; y; y = y->next) {
                    //bool isIntersect = runGJK(*x, *y);
                }
            }
        }
    }
}

void PhysicsSimulator::resolveContacts()
{
    while (_contacts) {
        // TODO
        _contacts = _contacts->next;
    }
}

void PhysicsSimulator::support(SupportPoint& point, const PhysicsShape& shapeA, const gx_common::GXMat4& transformA,
                const PhysicsShape& shapeB, const gx_common::GXMat4& transformB, const gx_common::GXVec3& direction)
{
    shapeA.getExtremePoint(point.extremePointA, transformA, direction);

    gx_common::GXVec3 invDirection(-direction.x, -direction.y, -direction.z);
    shapeB.getExtremePoint(point.extremePointB, transformB, invDirection);

    gx_common::gxSubVec3Vec3(point.difference, point.extremePointA, point.extremePointB);
}


void PhysicsSimulator::modifySimplex(Simplex& simplex, gx_common::GXVec3& direction)
{
    SupportPoint a = simplex.getA();
    SupportPoint b = simplex.getB();
    SupportPoint c = simplex.getC();

    gx_common::GXVec3 ab;
    gx_common::GXVec3 ac;
    gx_common::GXVec3 ao(-a.difference.x, -a.difference.y, -a.difference.z);
    gx_common::gxSubVec3Vec3(ab, b.difference, a.difference);
    gx_common::gxSubVec3Vec3(ac, c.difference, a.difference);

    gx_common::GXVec3 crossABxAC;
    gx_common::gxCrossVec3Vec3(crossABxAC, ab, ac);

    gx_common::GXVec3 v;
    gx_common::gxCrossVec3Vec3(v, ab, crossABxAC);
    if (simplex.dotTestWithAO(v)) {
        simplex.reset();
        simplex.addPoint(b);
        simplex.addPoint(a);

        gx_common::GXVec3 crossABxAO;
        gx_common::gxCrossVec3Vec3(crossABxAO, ab, ao);
        gx_common::gxCrossVec3Vec3(direction, crossABxAO, ab);
        return;
    }

    gx_common::gxCrossVec3Vec3(v, crossABxAC, ac);
    if (simplex.dotTestWithAO(v)) {
        simplex.reset();
        simplex.addPoint(c);
        simplex.addPoint(a);

        gx_common::GXVec3 crossACxAO;
        gx_common::gxCrossVec3Vec3(crossACxAO, ac, ao);
        gx_common::gxCrossVec3Vec3(direction, crossACxAO, ac);
        return;
    }

    simplex.reset();
    simplex.addPoint(c);
    simplex.addPoint(b);
    simplex.addPoint(a);
    direction = crossABxAC;
}

void PhysicsSimulator::addEdgeWithChecking(QVector<Edge>& edges, const SupportPoint& edgeStart,
                                           const SupportPoint& edgeEnd)
{
    int numEdges = edges.size();
    for (int i = 0; i < numEdges; i++) {
        Edge edge = edges[i];

        bool testA = gx_common::gxDistanceVec3Vec3(edge.points[0].difference,
                edgeEnd.difference) < DEFAULT_EDGE_EPSILON;

        bool testB = gx_common::gxDistanceVec3Vec3(edge.points[1].difference,
                edgeStart.difference) < DEFAULT_EDGE_EPSILON;

        if (testA && testB) {
            edges.remove(i);
            return;
        }
    }

    Edge e(edgeStart, edgeEnd);
    edges.push_back(e);
}

bool PhysicsSimulator::simpleWindingCheck(const Triangle& triangle, const gx_common::GXVec3& point)
{
    gx_common::GXVec3 triangleToPoint;
    gx_common::gxSubVec3Vec3(triangleToPoint, point, triangle.points[0].difference);

    return gx_common::gxDotVec3Fast(triangleToPoint, triangle.normal) > 0;
}

bool PhysicsSimulator::getContact(const Triangle& triangle, const PhysicsShapeLink& shapeA,
                                  const PhysicsShapeLink& shapeB)
{
    float distanceFromOrigin = gx_common::gxDotVec3Fast(triangle.normal, triangle.points[0].difference);

    gx_common::GXVec3 barycentricCoords;
    gx_common::GXVec3 point;
    gx_common::gxMulVec3Scalar(point, triangle.normal, distanceFromOrigin);

    gx_common::gxGetBarycentricCoords(barycentricCoords, point, triangle.points[0].difference,
                                      triangle.points[1].difference, triangle.points[2].difference);

    if (fabs(barycentricCoords.x) > 1.0f || fabs(barycentricCoords.y) > 1.0f || fabs(barycentricCoords.z) > 1.0f) {
        return false;
    }

    gx_common::GXVec3 v1;
    gx_common::gxMulVec3Scalar(v1, triangle.points[0].extremePointA, barycentricCoords.x);

    gx_common::GXVec3 v2;
    gx_common::gxMulVec3Scalar(v2, triangle.points[1].extremePointA, barycentricCoords.y);

    gx_common::GXVec3 v3;
    gx_common::gxMulVec3Scalar(v3, triangle.points[2].extremePointA, barycentricCoords.z);

    Contact contact;

    gx_common::gxSumVec3Vec3(contact.location, v1, v2);
    gx_common::gxSumVec3Vec3(contact.location, contact.location, v3);

    contact.a = shapeA.rigidBody;
    contact.b = shapeB.rigidBody;
    contact.normal = gx_common::gxCreateVec3(-triangle.normal.x, -triangle.normal.y, -triangle.normal.z);
    contact.penetration = distanceFromOrigin;
    contact.next = nullptr;

    contact.print();

    //Contact* c = _buffer.allocate(sizeof(Contact));
    //memcpy(c, &contact, sizeof(Contact));
/*
    if (_contacts) {
        c->next = _contacts;
    }

    _contacts = c;
*/
    return true;
}

} // namespace gx_engine
