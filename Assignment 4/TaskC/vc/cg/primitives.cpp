#include "primitives.hpp"

#include <iostream>

namespace cg
{
    namespace BasePrimitives
    {
        void generateCube(BaseMeshData& meshData)
        {
            meshData.attributes.clear();
            meshData.indices.clear();

            meshData.attributes = {
                {   {-1.0f, -1.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f } },
                {   { 1.0f, -1.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f } },
                {   { 1.0f,  1.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f } },
                {   {-1.0f,  1.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f } },

                {   { 1.0f, -1.0f,  1.0f }, { 1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
                {   { 1.0f, -1.0f, -1.0f }, { 1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } },
                {   { 1.0f,  1.0f, -1.0f }, { 1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
                {   { 1.0f,  1.0f,  1.0f }, { 1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } },

                {   { 1.0f, -1.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f } },
                {   {-1.0f, -1.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f } },
                {   {-1.0f,  1.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f } },
                {   { 1.0f,  1.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f } },

                {   {-1.0f, -1.0f, -1.0f }, {-1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
                {   {-1.0f, -1.0f,  1.0f }, {-1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } },
                {   {-1.0f,  1.0f,  1.0f }, {-1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
                {   {-1.0f,  1.0f, -1.0f }, {-1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } },

                {   { 1.0f,  1.0f,  1.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f } },
                {   {-1.0f,  1.0f,  1.0f }, { 0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f } },
                {   {-1.0f,  1.0f, -1.0f }, { 0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f } },
                {   { 1.0f,  1.0f, -1.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f } },

                {   { 1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f } },
                {   {-1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f } },
                {   {-1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f } },
                {   { 1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f } } };

            meshData.indices = {
                0, 1, 2,
                2, 3, 0,
                4, 5, 6,
                6, 7, 4,
                8, 9, 10,
                10, 11, 8,
                12, 13, 14,
                14, 15, 12,
                18, 17, 16,
                16, 19, 18,
                20, 22, 21,
                22, 20, 23,
            };
        }

        void generateQuad(BaseMeshData& meshData)
        {
            meshData.attributes = {
                {   {-1.0f, -1.0f,  0.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f } },
                {   { 1.0f, -1.0f,  0.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f } },
                {   { 1.0f,  1.0f,  0.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f } },
                {   {-1.0f,  1.0f,  0.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f } }
            };

            meshData.indices = {
                0, 1, 2,
                2, 3, 0,
            };
        }

        void generatePyramid(BaseMeshData& meshData)
        {
            meshData.attributes.clear();
            meshData.indices.clear();

            glm::vec3 n1 = glm::normalize(glm::vec3(1.f, 0.f, 1.f));
            glm::vec3 n2 = glm::normalize(glm::vec3(1.f, 0.f, -1.f));
            glm::vec3 n3 = glm::normalize(glm::vec3(-1.f, 0.f, -1.f));
            glm::vec3 n4 = glm::normalize(glm::vec3(-1.f, 0.f, 1.f));

            meshData.attributes = {
                {   { 0.0f, -0.5f,  1.0f }, n1, { 0.0f, 0.0f } },
                {   { 1.0f, -0.5f,  0.0f }, n1, { 1.0f, 0.0f } },
                {   { 0.0f,  0.5f,  0.0f }, n1, { 0.5f, 1.0f } },

                {   { 1.0f, -0.5f,  0.0f }, n2, { 0.0f, 0.0f } },
                {   { 0.0f, -0.5f, -1.0f }, n2, { 1.0f, 0.0f } },
                {   { 0.0f,  0.5f,  0.0f }, n2, { 0.5f, 1.0f } },

                {   { 0.0f, -0.5f, -1.0f }, n3, { 0.0f, 0.0f } },
                {   {-1.0f, -0.5f,  0.0f }, n3, { 1.0f, 0.0f } },
                {   { 0.0f,  0.5f,  0.0f }, n3, { 0.5f, 1.0f } },

                {   {-1.0f, -0.5f,  0.0f }, n4, { 0.0f, 0.0f } },
                {   { 0.0f, -0.5f,  1.0f }, n4, { 1.0f, 0.0f } },
                {   { 0.0f,  0.5f,  0.0f }, n4, { 0.5f, 1.0f } },

                {   { 0.0f, -0.5f, -1.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f } },
                {   {-1.0f, -0.5f,  0.0f }, { 0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f } },
                {   { 0.0f, -0.5f,  1.0f }, { 0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f } },
                {   { 1.0f, -0.5f,  0.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f } } };

            meshData.indices = {
                0, 1, 2,
                3, 4, 5,
                6, 7, 8,
                9, 10, 11,
                12, 13, 14,
                14, 15, 12,
            };
        }

        void generatePlane(const unsigned xSegs, const unsigned ySegs, BaseMeshData& meshData)
        {
            meshData.attributes.clear();
            meshData.indices.clear();

            meshData.attributes.reserve((xSegs + 1) * (ySegs + 1));
            meshData.indices.reserve(6 * xSegs * ySegs);

            const float xInc = 2.f / xSegs;
            const float yInc = 2.f / ySegs;

            float xPos = -1.f;
            for (unsigned int x = 0; x <= xSegs; ++x)
            {
                float yPos = -1.f;
                for (unsigned int y = 0; y <= ySegs; ++y)
                {
                    BaseVertex vertex;
                    vertex.position = glm::vec3(xPos, yPos, 0.f);
                    vertex.texcoord = glm::vec3(0.5f * xPos + 0.5f, 0.5f * yPos + 0.5f, 0.f);
                    vertex.normal = glm::vec3(0.f, 0.f, 1.f);
                    meshData.attributes.push_back(vertex);

                    yPos += yInc;

                    if (x == xSegs || y == ySegs) continue;

                    unsigned int i0 = x * (ySegs + 1) + y;
                    unsigned int i1 = x * (ySegs + 1) + (y + 1);
                    unsigned int i2 = (x + 1) * (ySegs + 1) + y;
                    unsigned int i3 = (x + 1) * (ySegs + 1) + (y + 1);

                    meshData.indices.push_back(i0); meshData.indices.push_back(i2); meshData.indices.push_back(i3);
                    meshData.indices.push_back(i0); meshData.indices.push_back(i3); meshData.indices.push_back(i1);
                }

                xPos += xInc;
            }
        }

        void generateSphere(const unsigned aSegs, const unsigned eSegs, BaseMeshData& meshData)
        {
            meshData.attributes.clear();
            meshData.indices.clear();

            meshData.attributes.reserve((eSegs + 1) * (aSegs + 1));
            meshData.indices.reserve(6 * eSegs * aSegs);

            const float aInc = 2 * glm::pi< float >() / aSegs;
            const float eInc = glm::pi< float >() / eSegs;

            float elevation = 0.f;
            for (unsigned int e = 0; e <= eSegs; ++e)
            {
                float azimuth = 0.f;
                for (unsigned int a = 0; a <= aSegs; ++a)
                {
                    BaseVertex vertex;
                    vertex.position = glm::vec3(glm::sin(elevation) * glm::cos(azimuth), glm::sin(elevation) * glm::sin(azimuth), glm::cos(elevation));
                    vertex.texcoord = glm::vec2(azimuth / (2 * glm::pi<float>()), elevation / glm::pi<float>());
                    vertex.normal = glm::normalize(vertex.position);
                    meshData.attributes.push_back(vertex);

                    azimuth += aInc;

                    if (e == eSegs || a == aSegs) continue;

                    unsigned int i0 = e * (aSegs + 1) + a;
                    unsigned int i1 = e * (aSegs + 1) + (a + 1);
                    unsigned int i2 = (e + 1) * (aSegs + 1) + a;
                    unsigned int i3 = (e + 1) * (aSegs + 1) + (a + 1);

                    //meshData.indices.push_back(i0); meshData.indices.push_back(i2); meshData.indices.push_back(i3);
                    //meshData.indices.push_back(i0); meshData.indices.push_back(i3); meshData.indices.push_back(i1);

                    meshData.indices.push_back(i1); meshData.indices.push_back(i0); meshData.indices.push_back(i2);
                    meshData.indices.push_back(i2); meshData.indices.push_back(i3); meshData.indices.push_back(i1);
                }

                elevation += eInc;
            }
        }


        void generateRing(const unsigned aSegs, const float rOuter, const float rInner, BaseMeshData& meshData)
        {
            meshData.attributes.clear();
            meshData.indices.clear();

            meshData.attributes.reserve(2 * aSegs + 1);
            meshData.indices.reserve(aSegs * 2 * 3);

            const float aInc = 2 * glm::pi< float >() / aSegs;
            const float rInc = rOuter - rInner;

            float radius = rInner;
            for (unsigned int r = 0; r <= 1; ++r)
            {
                float angle = 0.f;
                for (unsigned int a = 0; a <= aSegs; ++a)
                {
                    BaseVertex vertex;
                    vertex.position = radius * glm::vec3(glm::sin(angle), glm::cos(angle), 0.f);
                    vertex.normal = glm::vec3(0.f, 0.f, 1.f);
                    vertex.texcoord = glm::vec2(r, angle / (2 * glm::pi< float >()));
                    meshData.attributes.push_back(vertex);

                    angle += aInc;

                    if (a == aSegs) continue;

                    if (r == 1)
                    {
                        unsigned int i0 = r * (aSegs + 1) + a;
                        unsigned int i1 = r * (aSegs + 1) + a + 1;
                        unsigned int i2 = (r - 1) * (aSegs + 1) + a;
                        unsigned int i3 = (r - 1) * (aSegs + 1) + a + 1;

                        meshData.indices.push_back(i3); meshData.indices.push_back(i0); meshData.indices.push_back(i2);
                        meshData.indices.push_back(i1); meshData.indices.push_back(i0); meshData.indices.push_back(i3);
                    }
                }

                radius += rInc;
            }
        }

        void generateDisk(const unsigned aSegs, const unsigned rSegs, BaseMeshData& meshData)
        {
            meshData.attributes.clear();
            meshData.indices.clear();

            meshData.attributes.reserve(rSegs * (aSegs + 1) + 1);
            meshData.indices.reserve((rSegs - 1) * aSegs * 2 * 3 + aSegs * 3);

            const float aInc = 2 * glm::pi< float >() / aSegs;
            const float rInc = 1.0f / rSegs;

            BaseVertex origin;
            origin.position = glm::vec3(0.f);
            origin.normal = glm::vec3(0.f, 0.f, 1.f);
            origin.texcoord = glm::vec2(0.5f);
            meshData.attributes.push_back(origin);

            float radius = rInc;
            for (unsigned int r = 1; r <= rSegs; ++r)
            {
                float angle = 0.f;
                for (unsigned int a = 0; a <= aSegs; ++a)
                {
                    BaseVertex vertex;
                    vertex.position = radius * glm::vec3(glm::sin(angle), glm::cos(angle), 0.f);
                    vertex.normal = glm::vec3(0.f, 0.f, 1.f);
                    vertex.texcoord = 0.5f * glm::vec2(vertex.position.x, vertex.position.y) + glm::vec2(0.5f);
                    meshData.attributes.push_back(vertex);

                    angle += aInc;

                    if (a == aSegs) continue;

                    // innermost segment -> uses origin @idx 0
                    if (r == 1)
                    {
                        unsigned int i0 = a + 1;
                        unsigned int i1 = (a + 1) + 1;
                        meshData.indices.push_back(i1); meshData.indices.push_back(i0); meshData.indices.push_back(0);
                    }
                    // other segments
                    else
                    {
                        unsigned int i0 = (r - 1) * (aSegs + 1) + a + 1;
                        unsigned int i1 = (r - 1) * (aSegs + 1) + (a + 1) + 1;
                        unsigned int i2 = (r - 2) * (aSegs + 1) + a + 1;
                        unsigned int i3 = (r - 2) * (aSegs + 1) + (a + 1) + 1;

                        meshData.indices.push_back(i3); meshData.indices.push_back(i0); meshData.indices.push_back(i2);
                        meshData.indices.push_back(i1); meshData.indices.push_back(i0); meshData.indices.push_back(i3);
                    }
                }

                radius += rInc;
            }
        }

        void generateCylinder(const unsigned aSegs, const unsigned hSegs, BaseMeshData& meshData)
        {
            meshData.attributes.clear();
            meshData.indices.clear();
            meshData.attributes.reserve((hSegs + 1) * (aSegs + 1));
            meshData.indices.reserve(2 * 3 * hSegs * aSegs);

            const float aInc = 2 * glm::pi< float >() / aSegs;
            const float hInc = 1.0f / hSegs;

            float height = -0.5f;
            for (unsigned int h = 0; h <= hSegs; ++h)
            {
                float azimuth = 0.f;
                for (unsigned int a = 0; a <= aSegs; ++a)
                {
                    BaseVertex vertex;
                    vertex.position = glm::vec3(glm::sin(azimuth), height, glm::cos(azimuth));
                    vertex.normal = glm::normalize(glm::vec3(glm::sin(azimuth), 0.0f, glm::cos(azimuth)));
                    vertex.texcoord = glm::vec2(azimuth / (2 * glm::pi<float>()), height + 0.5f);
                    meshData.attributes.push_back(vertex);

                    azimuth += aInc;

                    if (h == hSegs || a == aSegs) continue;

                    unsigned i0 = h * (aSegs + 1) + a;
                    unsigned i1 = h * (aSegs + 1) + a + 1;
                    unsigned i2 = (h + 1) * (aSegs + 1) + a;
                    unsigned i3 = (h + 1) * (aSegs + 1) + a + 1;

                    meshData.indices.push_back(i0); meshData.indices.push_back(i3); meshData.indices.push_back(i2);
                    meshData.indices.push_back(i0); meshData.indices.push_back(i1); meshData.indices.push_back(i3);
                }

                height += hInc;
            }
        }

        void generateCone(const unsigned aSegs, const unsigned hSegs, BaseMeshData& meshData)
        {
            meshData.attributes.clear();
            meshData.indices.clear();
            meshData.attributes.reserve((hSegs + 1) * (aSegs + 1));
            meshData.indices.reserve(2 * hSegs * aSegs);

            const float aInc = 2 * glm::pi< float >() / aSegs;
            const float hInc = 1.0f / hSegs;
            const float rDec = 1.0f / hSegs;

            float height = -0.5f;
            float radius = 1.0f;
            for (unsigned int h = 0; h <= hSegs; ++h)
            {
                float azimuth = 0.f;
                for (unsigned int a = 0; a <= aSegs; ++a)
                {
                    BaseVertex vertex;
                    vertex.position = glm::vec3(radius * glm::sin(azimuth), height, radius * glm::cos(azimuth));
                    vertex.normal = glm::normalize(glm::vec3(glm::sin(azimuth), 0.0f, glm::cos(azimuth)));
                    vertex.texcoord = glm::vec2(azimuth / (2 * glm::pi<float>()), height + 0.5f);
                    meshData.attributes.push_back(vertex);

                    azimuth += aInc;

                    if (h == hSegs || a == aSegs) continue;

                    unsigned i0 = h * (aSegs + 1) + a;
                    unsigned i1 = h * (aSegs + 1) + a + 1;
                    unsigned i2 = (h + 1) * (aSegs + 1) + a;
                    unsigned i3 = (h + 1) * (aSegs + 1) + a + 1;

                    meshData.indices.push_back(i0); meshData.indices.push_back(i3); meshData.indices.push_back(i2);
                    meshData.indices.push_back(i0); meshData.indices.push_back(i1); meshData.indices.push_back(i3);
                }

                height += hInc;
                radius -= rDec;
            }
        }
    }
}