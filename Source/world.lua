local gfx <const> = playdate.graphics
local geom <const> = playdate.geometry

world = {
    orientations = {},
    cuboids = {},
    -- 1 from the front
    -- 2 from the right
    -- 3 from the back
    -- 4 from the left
    orientation = 1
}
world.orientations[1] = {}
world.orientations[2] = {}
world.orientations[3] = {}
world.orientations[4] = {}

-- convert player coords to world coords
function world:flatToWorld(x, z)
    if self.orientation == 1 then
        return x, z
    elseif self.orientation == 2 then
        return z, -x
    elseif self.orientation == 3 then
        return -x, -z
    elseif self.orientation == 4 then
        return -z, x
    end
end

function world:worldToFlat(x, z)
    if self.orientation == 1 then
        return x, z
    elseif self.orientation == 2 then
        return -z, x
    elseif self.orientation == 3 then
        return -x, -z
    elseif self.orientation == 4 then
        return z, -x
    end
end

table.insert(world.cuboids, fez.cuboid.new(-64, -128, -64, 128, 800, 128, 0))
table.insert(world.cuboids, fez.cuboid.new(-288, 0, -96, 416, 32, 192, 1))
table.insert(world.cuboids, fez.cuboid.new(160, -32, -160, 64, 32, 64, 1))
table.insert(world.cuboids, fez.cuboid.new(288, -64, -32, 64, 32, 64, 1))

-- table.insert(world.cuboids, fez.cuboid.new(-160, -192, -224, 64, 32, 64, 1))
-- table.insert(world.cuboids, fez.cuboid.new(-32, -320, -288, 224, 32, 64, 1))

for _, cuboid in pairs(world.cuboids) do

    local rectFront = geom.rect.new(cuboid.x, cuboid.y, cuboid.width, cuboid.height)
    local faceFront = {
        rect = rectFront,
        z = cuboid.z
    }
    table.insert(world.orientations[1], faceFront)

    local rectLeft = geom.rect.new(-cuboid.z - cuboid.depth, cuboid.y, cuboid.depth, cuboid.height)
    local faceLeft = {
        rect = rectLeft,
        z = cuboid.x
    }
    table.insert(world.orientations[2], faceLeft)

    local rectBack = geom.rect.new(-cuboid.x - cuboid.width, cuboid.y, cuboid.width, cuboid.height)
    local faceBack = {
        rect = rectBack,
        z = -cuboid.z - cuboid.depth
    }
    table.insert(world.orientations[3], faceBack)

    local rectRight = geom.rect.new(cuboid.z, cuboid.y, cuboid.depth, cuboid.height)
    local faceRight = {
        rect = rectRight,
        z = -cuboid.x - cuboid.width
    }
    table.insert(world.orientations[4], faceRight)
end

function world:draw(cam)
    for _, cuboid in pairs(self.cuboids) do
        fez.drawCuboid(cuboid, cam)
    end
end

function world:debugDraw()
    for _, face in pairs(self.orientations[self.orientation]) do
        gfx.drawRect(face.rect)
    end
end
