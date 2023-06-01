import "controller"

local gfx <const> = playdate.graphics
local transitionFrames <const> = 13

scene = {
    transitionNone = 0,
    transitionFade = 1,
    transtionFrame = 1,
    isTransitioning = false,
    nextScene = "",
}

local baseFadeImage = gfx.image.new(400, 240, gfx.kColorBlack)
local fadeSprite = gfx.sprite.new()
fadeSprite:setZIndex(100)
fadeSprite:setCenter(0, 0)
fadeSprite:setIgnoresDrawOffset(true)

local fadeImages = {}

for i=1, transitionFrames do
    fadeImages[i] = baseFadeImage:fadedImage(i / transitionFrames, gfx.image.kDitherTypeBayer8x8)
end

function scene.addScene(name, scene_)
    scene[name] = scene_
end

function scene.setScene(name, transition)
    if transition == scene.transitionNone then
        if scene.activeScene then
            scene.activeScene:onUnload()
        end
        scene.activeScene = scene[name]
        scene.activeScene:onLoad()
    elseif transition == scene.transitionFade then
        scene.isTransitioning = true
        scene.transitionFrame = 1
        controller.active = false
        fadeSprite:add()
        scene.nextScene = name
    end
end

function scene.update()
    if not scene.isTransitioning then
        if scene.activeScene then
            scene.activeScene:update_()
            for key, sprite in pairs(scene.activeScene.sprites) do
                if sprite.dead then
                    sprite:remove()
                    table.remove(scene.activeScene.sprites, key)
                end
            end
        end
    else
        if scene.transitionFrame < transitionFrames * 2 then
            if scene.transitionFrame < transitionFrames then
                local img = fadeImages[scene.transitionFrame]
                fadeSprite:setImage(img)
                scene.transitionFrame = scene.transitionFrame + 1
            else
                -- set to new frame
                if scene.transitionFrame == transitionFrames + 1 then
                    if scene.activeScene then
                        scene.activeScene:onUnload()
                    end
                    scene.activeScene = scene[scene.nextScene]
                    scene.activeScene:onLoad()
                end

                local frame = scene.transitionFrame - transitionFrames
                frame = transitionFrames - frame

                local img = fadeImages[frame]
                fadeSprite:setImage(img)
                scene.transitionFrame = scene.transitionFrame + 1
            end
        else
            fadeSprite:remove()
            scene.transtionFrame = 1
            scene.isTransitioning = false
            controller.active = true

        end
        -- fadeout
        -- switch the active scene
        -- fadaein
    end
end

function scene.makeScene()
    local cn = {
        active = false,
        sprites = {},
        update = nil
    }

    function cn:update_()
        if self.update then
            self:update()
        end
    end

    function cn:onLoad()
        self.active = true
        for _, sprite in pairs(self.sprites) do
            sprite:add()
        end
    end

    function cn:onUnload()
        self.active = false
        for _, sprite in pairs(self.sprites) do
            sprite:remove()
        end
    end

    function cn:addSprite(sprite)
        table.insert(self.sprites, sprite)
        if self.active then
            sprite:add()
        end
    end

    return cn
end

