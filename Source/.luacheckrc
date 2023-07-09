stds.fez = {
    globals = {
        fez = {
            fields = {
                cuboid = {
                    fields = {
                        "new"
                    }
                },
                camera = {
                    fields = {
                        "new",
                        "aimToSubject",
                        "rotateAround"
                    }
                },
                texture = {
                    fields = {
                        "new",
                    }
                },
                "drawCuboid",
                "drawBuffered",
            }
        }
    }
}

stds.luqua = {
    globals = {
        scene = {
            fields = {
                "transitionNone",
                "transitionFade",
                "addScene",
                "setScene",
                "update",
                "makeScene",
            }
        },
        controller = {
            fields = {
                "active",
                "getCrankPosition",
                "getCrankChange",
                "buttonIsPressed"
            }
        }
    }
}

stds.project = {
    globals = {
        world = {
            other_fields = true
        },
        player = {
            other_fields = true
        }
    }
}


std="max+playdate+luqua+fez+project"
operators = {"+=", "-=", "*=", "/="}
