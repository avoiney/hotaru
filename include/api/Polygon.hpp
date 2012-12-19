#ifndef _API_POLYGON_HPP_
#define _API_POLYGON_HPP_

#include <glm/glm.hpp>
typedef glm::vec3 Col3f;

class Polygon {
public:
    Col3f color;

    Polygon(const Col3f& color, bool inner = true):
        color(color), m_bInner(inner) {
    }

    void addVertex(const glm::vec2& vertex) {
        m_Vertices.push_back(vertex);
    }

    const glm::vec2& getVertex(size_t i) const {
        return m_Vertices[i];
    }

    size_t size() const {
        return m_Vertices.size();
    }
    
    bool isInner() const {
        return m_bInner;
    }

    const std::vector<glm::vec2>& getVertices() const {
        return m_Vertices;
    }

private:
    bool m_bInner;
    std::vector<glm::vec2> m_Vertices;
};

#endif