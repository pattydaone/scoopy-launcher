### Scoopy Launcher

This project is inspired by sway-desktop-launcher and attempts to function in the same way.

There is currently nothing to customize. Maybe I'll get to that. But it should function in 
the sense that it should launch applications as the name implies.

The code in this repo is most certainly horrific as I am an ill experienced developer in general, much less in c++, 
but that's ok, this project was intended to be for my own growth and experience. I've learned a lot in creating this!!

I'll probably do more work on it maybe, but we shall see. I suppose no one will really read this, but just in case, here's 
a quick rundown:

There is no release, you'll have to compile it yourself. Simply clone the repo, cd into the /src folder, and run `make`.

From there, you can do whatever you want with the produced binary. I use it in sway with kitty, so I have a config that looks 
something like: 

`set $menu exec $term -c ~/.config/kitty/launcherKitty.conf --app-id=launcher /home/patrick/Projects/launcher/bin/main`

Then I bind `$menu` to a keybind and it works ! How exciting!

If anyone wants to open issues for bugs/suggestions feel free. I might get to them, I might not. Pull requests (as if!) will probably go ignored.
It's my project and I intend to keep it as such. If you want to add onto it, fork it. Bye bye now !
