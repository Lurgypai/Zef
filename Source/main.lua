-- add in gomez
-- update camera (add cheap panning)
-- add in collisions
-- add in time freeze/stage rotation
-- add in stage collider generation
-- images?

import "CoreLibs/object"
import "CoreLibs/graphics"
import "CoreLibs/sprites"
import "CoreLibs/timer"
import "CoreLibs/animation"

import "world"
import "player"

local cam = fez.camera.new(0, 0, 0, 0)
local offsetX = 0

local texture = fez.texture.new("images/tiles/basic/basic")
-- local shadedTexture = fez.texture.new("images/tiles/basic/basic_shaded")
--local textCuboid = fez.cuboid.new(-288, -96, 0, 32, 96, 64)

function playdate.update()
    -- update player
    player:update()
    local x, z = world:flatToWorld(player.x, player.z)
    if not player.isFrozen then
        cam:aimToSubject(x, player.y, z, 1000, (3.1415926535898 / 2) * (world.orientation - 1))
    else
        cam:aimToSubject(x, player.y, z, 1000, player.worldRotation * 3.1415926535898 / 180)
    end

    playdate.graphics.clear()
    --draw world
    playdate.graphics.setDrawOffset(0, 0)
    world:bufferTextured(texture, cam)
    fez.drawBuffered(cam)

    --draw player
    if not player.isFrozen then
        offsetX = world:worldToFlat(cam.x, cam.z)
        playdate.graphics.setDrawOffset(-offsetX, -cam.y)
        local flip = playdate.graphics.kImageUnflipped
        if player.animState.flipped then flip = playdate.graphics.kImageFlippedX end
        player:getImage():draw(player.x - 32, player.y - 64, flip)
    else
        playdate.graphics.setDrawOffset(0, 0)
        local flip = playdate.graphics.kImageUnflipped
        if player.animState.flipped then flip = playdate.graphics.kImageFlippedX end
        player:getImage():draw(200-32, 120-64, flip)
    end

end

--[[
function playdate.debugDraw()
    playdate.graphics.setDrawOffset(-offsetX, -cam.y)
    world:debugDraw()
end
]]--

