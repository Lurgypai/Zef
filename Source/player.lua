local gfx <const> = playdate.graphics
local snd <const> = playdate.sound

-- set up player sprite
player = gfx.sprite.new()
player:setCenter(0.5, 1.0)
player:setCollideRect(26, 48, 12, 16)
player:moveTo(-272, -1)

-- set up player gfx
player.imageTable = gfx.imagetable.new("images/gomez_big")
if player.imageTable == nil then
    print('Error loading table')
end

player.idleAnimation = gfx.animation.loop.new(100, player.imageTable)
player.idleAnimation.startFrame = 1
player.idleAnimation.endFrame = 1

player.runAnimation = gfx.animation.loop.new(100, player.imageTable)
player.runAnimation.startFrame = 2
player.runAnimation.endFrame = 7

player.turnaroundAnimation = gfx.animation.loop.new(100, player.imageTable)
player.turnaroundAnimation.startFrame = 8
player.turnaroundAnimation.endFrame = 12
player.turnaroundAnimation.shouldLoop = false

player.startJumpAnimation = gfx.animation.loop.new(100, player.imageTable)
player.startJumpAnimation.startFrame = 13
player.startJumpAnimation.endFrame = 17
player.startJumpAnimation.shouldLoop = false

player.startFallingAnimation = gfx.animation.loop.new(100, player.imageTable)
player.startFallingAnimation.startFrame = 18
player.startFallingAnimation.endFrame = 18
player.startFallingAnimation.shouldLoop = false

player.landAnimation = gfx.animation.loop.new(50, player.imageTable)
player.landAnimation.startFrame = 19
player.landAnimation.endFrame = 20
player.landAnimation.shouldLoop = false

player.currAnimation = player.idleAnimation

-- player sfx

player.jumpSound = snd.sampleplayer.new("sounds/jump")
player.landSound = snd.sampleplayer.new("sounds/land")
player.turnSound = snd.sampleplayer.new("sounds/turnaround")
player.stepLeftSound = snd.sampleplayer.new("sounds/left")
player.stepRightSound = snd.sampleplayer.new("sounds/right")
player.rotateLeftSound = snd.sampleplayer.new("sounds/rotateleft")
player.rotateRightSound = snd.sampleplayer.new("sounds/rotateright")


-- player physics
player.worldRotation = 0
player.isSettling = false
player.targetRotation = 0
player.z = 0
player.gravity = 1
player.maxFallSpeed = 15
player.startJumpSpeed = 10
player.additionalJumpSpeed = 1
player.isFrozen = false
player.prevPos = {
    x = 0,
    y = 0
}
player.vel = {
    x = 0,
    y = 0
}
player.grounded = true
player.isJumping = false
player.jumpFrame = 0
player.jumpFrameMax = 3
player.nextJumpSpeed = 0
player.groundedAccel = 0.8
player.groundedDecel = 1
player.airAccel = 0.3
player.maxHorizSpeed = 5
player.animState = {
    flipped = false,
    prevVel = {
        x = 0,
        y = 0
    },
    prevGrounded = false,
    prevJumping = false,
    turnaroundThreshold = 2,
    currAnimTag = "idle"
}

-- player update
function player:update()
    ---------- WORLD MAGIC
    if playdate.buttonJustPressed(playdate.kButtonB) then
        if not player.isFrozen then
            player.isFrozen = true
            player.currAnimation.pause = true
            player.isSettling = false
            player.rotateLeftSound:play()
        else
            player.isSettling = true
            if player.worldRotation < 135 and player.worldRotation >= 45 then
                player.targetRotation = 90
            elseif player.worldRotation < 225 and player.worldRotation >= 135 then
                player.targetRotation = 180
            elseif player.worldRotation < 315 and player.worldRotation >= 225 then
                player.targetRotation = 270
            else
                player.targetRotation = 0
            end
            player.rotateRightSound:play()
        end
    end

    if player.isFrozen then
        if not player.isSettling then
            player.worldRotation += playdate.getCrankChange()
            if player.worldRotation >= 360 then player.worldRotation -= 360 end
            if player.worldRotation < 0 then player.worldRotation += 360 end
        else
            if player.worldRotation >= 315 then player.worldRotation -= 360 end
            player.worldRotation += (player.targetRotation - player.worldRotation) / 8
            if math.abs(player.worldRotation - player.targetRotation) < 0.5 then
                player.worldRotation = player.targetRotation
                local currWorldX, currWorldZ = world:flatToWorld(player.x, player.z)
                player.currAnimation.pause = false

                if player.worldRotation == 0 then world.orientation = 1 end
                if player.worldRotation == 90 then world.orientation = 2 end
                if player.worldRotation == 180 then world.orientation = 3 end
                if player.worldRotation == 270 then world.orientation = 4 end

                local playerX, playerZ = world:worldToFlat(currWorldX, currWorldZ)
                player:moveTo(playerX, player.y)
                player.z = playerZ
                player.isFrozen = false
            end
        end
    else
        ---------- MOVEMENT
        -- left right
        local horizDir = 0
        if playdate.buttonIsPressed(playdate.kButtonLeft) then horizDir -= 1 end
        if playdate.buttonIsPressed(playdate.kButtonRight) then horizDir += 1 end

        -- controlled movement
        if horizDir ~= 0 then
            if player.grounded then player.vel.x += horizDir * player.groundedAccel
            else player.vel.x += horizDir * player.airAccel end
        -- natural grounded decel
        elseif player.grounded then
            if player.vel.x > player.groundedDecel then player.vel.x -= player.groundedDecel
            elseif player.vel.x < -player.groundedDecel then player.vel.x += player.groundedDecel
            else player.vel.x = 0 end
        end

        if player.vel.x > player.maxHorizSpeed then player.vel.x = player.maxHorizSpeed
        elseif player.vel.x < -player.maxHorizSpeed then player.vel.x = -player.maxHorizSpeed end

        -- jumping
        if playdate.buttonJustPressed(playdate.kButtonA) then
            if player.grounded then
                if not player.isJumping then
                    player.isJumping = true
                    player.jumpFrame = 0
                    player.nextJumpSpeed = player.startJumpSpeed
                end
            end
        end
        if playdate.buttonIsPressed(playdate.kButtonA) then
            if player.grounded then
                if player.isJumping then
                    player.nextJumpSpeed += player.additionalJumpSpeed
                end
            end
        end

        if player.isJumping then
            if player.jumpFrame ~= player.jumpFrameMax then
                player.jumpFrame += 1
            else
                player.vel.y = -player.nextJumpSpeed
                player.isJumping = false
            end
        end


        player.vel.y += player.gravity
        if player.vel.y > player.maxFallSpeed then
            player.vel.y = player.maxFallSpeed
        end

        -- collision detection
        player.grounded = false
        local newPos = {
            x = 0,
            y = 0,
        }
        newPos.x = player.x + player.vel.x
        newPos.y = player.y + player.vel.y

        player:moveTo(newPos.x, newPos.y)

        for i, face in pairs(world.orientations[world.orientation]) do
            local playerRect = player:getCollideRect()
            local boundsX, boundsY = player:getBounds()
            playerRect.x += boundsX
            playerRect.y += boundsY

            if playerRect:intersects(face.rect) then
                if player.vel.y > 0 and player.prevPos.y <= face.rect.y then
                    player.vel.y = 0
                    player.grounded = true
                    player:moveTo(player.x, face.rect.y)
                    player.z = face.z + 16
                end
            end
        end

        if player.y > 500 then
            player.x = 0
            player.y = 0
            player.vel.y = 0
        end

        --------------- ANIMATION

        -- just landed
        if player.animState.prevGrounded ~= player.grounded and player.grounded then
            player.animState.currAnimTag = "landing"
            player.currAnimation = player.landAnimation
            player.currAnimation.frame = player.currAnimation.startFrame

            player.landSound:play()
        -- grounded
        elseif player.grounded then
            -- started jumping
            if player.animState.prevJumping ~= player.isJumping and player.isJumping then
                player.animState.currAnimTag = "jumping"
                player.currAnimation = player.startJumpAnimation
                player.currAnimation.frame = player.currAnimation.startFrame

                player.jumpSound:play()
            -- staying grounded
            else
                -- pressed turnaround
                if (playdate.buttonJustPressed(playdate.kButtonLeft) and
                        player.vel.x >= player.animState.turnaroundThreshold) or
                    (playdate.buttonJustPressed(playdate.kButtonRight) and
                        player.vel.x <= -player.animState.turnaroundThreshold) then
                    player.currAnimation = player.turnaroundAnimation
                    player.currAnimation.frame = player.currAnimation.startFrame
                    player.animState.currAnimTag = "turning"
                    player.turnSound:play()

                elseif player.vel.x ~= player.animState.prevVel.x then
                    -- started moving
                    if player.animState.prevVel.x == 0 and player.vel.x ~= 0 then
                        -- don't interrupt a turn
                        if player.animState.currAnimTag ~= "turning" then
                            player.animState.currAnimTag = "running"
                            player.currAnimation = player.runAnimation
                            player.currAnimation.frame = player.currAnimation.startFrame
                        end

                    -- stopped moving
                    elseif player.vel.x == 0 then
                        if player.animState.currAnimTag ~= "turning" then
                            player.animState.currAnimTag = "idle"
                            player.currAnimation = player.idleAnimation
                            player.currAnimation.frame = player.currAnimation.startFrame
                            -- we don't want to keep turning
                        elseif not
                            ((playdate.buttonIsPressed(playdate.kButtonLeft)
                                and player.animState.prevVel.x > 0) or
                            (playdate.buttonIsPressed(playdate.kButtonRight)
                                and player.animState.prevVel.x < 0)) then
                            player.animState.currAnimTag = "idle"
                            player.currAnimation = player.idleAnimation
                            player.currAnimation.frame = player.currAnimation.startFrame
                        end
                    end
                end
            end
        -- airborn
        else
            -- just started falling
            if player.animState.prevVel.y <= 0 and player.vel.y > 0 then
                player.animState.currAnimTag = "falling"
                player.currAnimation = player.startFallingAnimation
                player.currAnimation.frame = player.currAnimation.startFrame
            end
        end

        -- if the landing animation finished
        if not player.currAnimation:isValid() then
            if player.animState.currAnimTag == "landing" and player.grounded then
                if player.vel.x ~= 0 then
                    player.currAnimation = player.runAnimation
                    player.currAnimation.frame = player.currAnimation.startFrame
                    player.animState.currAnimTag = "running"
                else
                    player.currAnimation = player.idleAnimation
                    player.currAnimation.frame = player.currAnimation.startFrame
                    player.animState.currAnimTag = "idle"
                end
            elseif player.animState.currAnimTag == "turning" then
                if player.vel.x ~= 0 then
                    player.currAnimation = player.runAnimation
                    player.currAnimation.frame = player.currAnimation.startFrame
                    player.animState.currAnimTag = "running"
                else
                    player.currAnimation = player.idleAnimation
                    player.currAnimation.frame = player.currAnimation.startFrame
                    player.animState.currAnimTag = "idle"
                end
            end
        end

        if player.animState.currAnimTag ~= 'turning' then
            if player.vel.x < 0 then player.animState.flipped = true
            elseif player.vel.x > 0 then player.animState.flipped = false end
        else
            if playdate.buttonJustPressed(playdate.kButtonLeft) then
                player.animState.flipped = false
            elseif playdate.buttonJustPressed(playdate.kButtonRight) then
                player.animState.flipped = true
            end
        end

        player:setImage(player.currAnimation:image())

        if player.animState.currAnimTag == 'running' then
            if player.currAnimation.frame == 2 then
                if not player.stepLeftSound:isPlaying() then player.stepLeftSound:play() end
            elseif player.currAnimation.frame ==  5 then
                if not player.stepRightSound:isPlaying() then player.stepRightSound:play() end
            end
        end
    end

    player.animState.prevVel.x = player.vel.x
    player.animState.prevVel.y = player.vel.y
    player.animState.prevGrounded = player.grounded
    player.animState.prevJumping = player.isJumping

    player.prevPos.x = player.x
    player.prevPos.y = player.y
end

player:add()
