controller = {
    active = true,
}

function controller.getCrankPosition()
    if controller.active then
        return playdate.getCrankPosition()
    else
        return 0
    end
end

function controller.getCrankChange()
    if controller.active then
        return playdate.getCrankChange()
    else
        return 0
    end
end

function controller.buttonIsPressed(btn)
    if controller.active then
        return playdate.buttonIsPressed(btn)
    else
        return false
    end
end



