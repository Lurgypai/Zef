# Zef, a Fez implentation for playdate

Yo, I was playing Fez and thought this would be fun. Cut a lot of corners to get a demo out quickly, but it works well enough.

### Known Issues
I haven't implemented z indexing for the rendering, so the player is always on top, but the cuboids are properly ordered.
</br>
The colliders are generated for each view. The generation code just generates them without taking into account if there is stuff obstructing them. This would be a single up front cost that could be added without issue.
</br>
The player only exists in 2D. This is not how the original game works, and it was done to make it simpler, and allow me to use the standard Playdate tools. This is definitely not the best way to do this, and I think I from scratch implementation using full 3D might just work better. Oh well.

