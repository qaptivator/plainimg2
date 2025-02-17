from PIL import Image, ImageTk
import tkinter as tk
from tkinter import filedialog
from tkinter import ttk
import sys
import os
import webbrowser
import requests
#from ctypes import windll, c_int

DEFAULT_SIZE = (800, 600)
MIN_SIZE = (200, 150)
WINDOW_TITLE = "plainIMG"
WINDOW_TITLE_PINNED = "plainIMG [TOP]"
BG_COLOR_INIT = "white"
GITHUB_REPO_URL = "https://github.com/qaptivator/plainimg"
GITHUB_VERSION_TXT_URL = "https://raw.githubusercontent.com/qaptivator/plainimg/refs/heads/main/version.txt"

main_font = ("Consolas", 24, "bold")
about_font = ("Consolas", 12, "bold")
about_font_small = ("Consolas", 10)
about_font_alt = ("Consolas", 10, "italic")

if os.path.exists("version.txt"):
    with open("version.txt", "r") as f:
        VERSION = f.read().strip()
else:
    VERSION = "0.0.0" # indicates that there is no version.txt file

# this should probably be inside a function, instead of global scope...
# loading, no_connection, request_error, up_to_date, update_available
#VERSION_STATUS = "loading"

#try:
#    version_request = requests.get(GITHUB_VERSION_TXT_URL, timeout=5)
#    version_request.raise_for_status()
#
#    version_content = version_request.text()
#    if version_content:
#        if version_content == VERSION:
#            VERSION_STATUS = "up_to_date"
#        else:
#            VERSION_STATUS = "update_available"
#    else:
#        VERSION_STATUS = "request_error"
#
#except (requests.ConnectionError, requests.Timeout) as exception:
#    VERSION_STATUS = "no_connection"
#
#except (requests.HTTPError) as exception:
#    VERSION_STATUS = "request_error"

# lite mode makes the window as basic as posible, removing the title bar and regular handling provided by windows
# i have added an option for this because i feel like this functionality can break really easily, because i handle everything in the window now
LITE_MODE = False

class ImageViewer:
    def __init__(self, root, image_path=None):
        self.root = root
        self.image_path = image_path
        self.platform = {
            'win': sys.platform.startswith("win"),
        }

        self.always_on_top = tk.BooleanVar(value=True)
        self.keep_aspect_ratio = tk.BooleanVar(value=True)
        self.use_black_bg = tk.BooleanVar(value=False)
        self.use_antialiasing = tk.BooleanVar(value=False)
        #self.image_opened = self.image_path is not None
        #self.img = Image.open(image_path)
        #self.img_original = self.img.copy() 
        #self.photo = ImageTk.PhotoImage(self.img)
        #self.open_image()
        
        #self.root.wm_attributes("-fullscreen", True)
        #self.root.wm_attributes('-type', 'splash')
        #https://stackoverflow.com/questions/39529600/remove-titlebar-without-overrideredirect-using-tkinter
        if LITE_MODE:
            self.root.overrideredirect(True)

            # drag
            self.move_x = 0
            self.move_y = 0
            #self.root.bind("<ButtonPress-1>", self.start_move)
            #self.root.bind("<B1-Motion>", self.on_move)

            # resize
            self.resizing = False
            self.resize_start_x = 0
            self.resize_start_y = 0
            self.resize_start_width = 0
            self.resize_start_height = 0
            #self.root.bind("<Motion>", self.check_resize_area)
            #self.root.bind("<ButtonPress-1>", self.start_resize)
            #self.root.bind("<B1-Motion>", self.do_resize)
            #self.root.bind("<ButtonRelease-1>", self.stop_resize)
            self.root.bind("<Motion>", self.on_motion)
            self.root.bind("<ButtonPress-1>", self.on_buttonpress_1)
            self.root.bind("<B1-Motion>", self.on_b1_motion)
            self.root.bind("<ButtonRelease-1>", self.on_buttonrelease_1)

            # Apply rounded corners (Windows only)
            #if self.platform.get('win'):
            #    hwnd = windll.user32.GetParent(self.root.winfo_id())
            #    windll.dwmapi.DwmSetWindowAttribute(hwnd, 2, c_int(1), 4) # TRUE is literally just 1 (windll.user32.TRUE)


        self.canvas = tk.Canvas(root, width=DEFAULT_SIZE[0], height=DEFAULT_SIZE[1], bg=BG_COLOR_INIT, highlightthickness=0)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.open_image()

        self.canvas.bind("<Configure>", self.resize_image)
        self.root.bind("q", self.quit_dummy)
        self.root.bind("o", self.open_image_command)
        self.root.bind("c", self.close_image)
        self.root.bind("t", self.toggle_always_on_top)
        self.root.bind("a", self.toggle_keep_aspect_ratio)
        self.root.bind("r", self.resize_window_to_image)
        self.root.bind("b", self.toggle_use_black_bg)
        self.root.bind("l", self.toggle_use_antialiasing)

        self.menu = tk.Menu(root, tearoff=0)
        self.menu.add_command(label="Open Image... (O)", command=self.open_image_command)
        self.menu.add_command(label="Close Image (C)", command=self.close_image)
        self.menu.add_checkbutton(label="Window always on top (T)", variable=self.always_on_top, command=self.toggle_always_on_top)
        self.menu.add_checkbutton(label="Keep aspect ratio (A)", variable=self.keep_aspect_ratio, command=self.toggle_keep_aspect_ratio)
        self.menu.add_command(label="Resize window to image (R)", command=self.resize_window_to_image)
        self.menu.add_checkbutton(label="Use black background (B)", variable=self.use_black_bg, command=self.toggle_use_black_bg)
        self.menu.add_checkbutton(label="Use antialiasing (L)", variable=self.use_antialiasing, command=self.toggle_use_antialiasing)
        self.menu.add_separator()
        self.menu.add_command(label="About", command=self.open_about_popup)
        self.menu.add_command(label="Quit (Q)", command=self.quit_dummy)
        self.root.bind("<Button-3>", self.show_menu)

        self.update_always_on_top()
        self.update_canvas()

    def image_opened(self):
        return self.image_path is not None
    
    def centerize_widget(self, widget, parent):
        width, height = widget.winfo_width(), widget.winfo_height()
        root_x, root_y = parent.winfo_x(), parent.winfo_y()
        root_width, root_height = parent.winfo_width(), parent.winfo_height()
        x = root_x + (root_width - width) // 2
        y = root_y + (root_height - height) // 2
        widget.geometry(f"+{x}+{y}")
    
    # oh wait i couldve fixed the text positioning problem with just wait_visibility, ugh...
    # ABOUT
    def open_about_popup(self):
        popup = tk.Toplevel(self.root)
        popup.title("About")
        popup.geometry("300x110")
        popup.resizable(False, False)

        #if self.platform() == "win":
            #popup.wm_attributes("-toolwindow", True) # this hides the icon on windows
            #popup.iconbitmap("") # this also works i think
        
        tk.Label(popup, text="plainIMG", font=about_font, anchor="w").pack(fill="both")
        tk.Label(popup, text="as simple as it gets for an image viewer", font=about_font_alt, anchor="w").pack(fill="both")
        tk.Label(popup, text=f"version: {VERSION}", font=about_font_small, anchor="w").pack(fill="both")
        tk.Label(popup, text="made by qaptivator, licensed under MIT", font=about_font_small, anchor="w").pack(fill="both")

        #tk.Label(popup, text="Loading latest version number from repository...", font=about_font_alt, anchor="w").pack(fill="both")
        #if VERSION_STATUS == "loading":
        #    tk.Label(popup, text="Loading latest version number form repository...", font=about_font_small, anchor="w").pack(fill="both")
        #elif VERSION_STATUS == "no_connection":
        #    tk.Label(popup, text="Couldn't fetch", font=about_font_small, anchor="w").pack(fill="both")

        tk.Button(popup, text="GitHub Repository", command=lambda: webbrowser.open(GITHUB_REPO_URL), fg="blue", anchor="w").pack(fill="x", side="left", expand=True)
        tk.Button(popup, text="Close", command=popup.destroy, anchor="w").pack(fill="x", side="left", expand=True)

        popup.transient(self.root)
        popup.iconbitmap('icon.ico')

        popup.wait_visibility()
        popup.update_idletasks()
        self.centerize_widget(popup, self.root)

        #popup.transient(self.root)
        #popup.grab_set() # OPTIONAL. makes this popup act like a modal (which is going to be annoying in my opinion)
    
    # WINDOW
    def on_motion(self, event):
        self.check_resize_area(event)

    def on_b1_motion(self, event):
        self.on_move(event)
        self.do_resize(event)

    def on_buttonpress_1(self, event):
        self.start_move(event)
        self.start_resize(event)

    def on_buttonrelease_1(self, event):
        self.stop_resize(event)

    def check_resize_area(self, event):
        """Detects if the cursor is near the edges to enable resizing."""
        border_thickness = 10  # Resize area width
        x, y, width, height = event.x, event.y, self.root.winfo_width(), self.root.winfo_height()

        if x >= width - border_thickness and y >= height - border_thickness:
            self.root.config(cursor="bottom_right_corner")  # Bottom-right resize
            self.resizing = "bottom_right"
        elif x >= width - border_thickness:
            self.root.config(cursor="right_side")  # Right-side resize
            self.resizing = "right"
        elif y >= height - border_thickness:
            self.root.config(cursor="bottom_side")  # Bottom resize
            self.resizing = "bottom"
        else:
            self.root.config(cursor="arrow")
            self.resizing = False

    def start_resize(self, event):
        """Start resizing when clicking the border."""
        self.resize_start_x, self.resize_start_y = event.x, event.y
        self.resize_start_width, self.resize_start_height = self.root.winfo_width(), self.root.winfo_height()

    def do_resize(self, event):
        """Resize the window while dragging the border."""
        if not self.resizing:
            return

        if self.resizing in ("right", "bottom_right"):
            new_width = max(MIN_SIZE[0], self.resize_start_width + (event.x - self.resize_start_x))
            self.root.geometry(f"{new_width}x{self.root.winfo_height()}")

        if self.resizing in ("bottom", "bottom_right"):
            new_height = max(MIN_SIZE[1], self.resize_start_height + (event.y - self.resize_start_y))
            self.root.geometry(f"{self.root.winfo_width()}x{new_height}")

    def stop_resize(self, event):
        """Stop resizing after releasing the mouse button."""
        self.resizing = False
        self.root.config(cursor="arrow")
    
    def start_move(self, event):
        self.move_x = event.x
        self.move_y = event.y

    def on_move(self, event):
        if not self.resizing:
            self.root.geometry(f"+{event.x_root - self.move_x}+{event.y_root - self.move_y}")

    # MENU BUTTONS
    def quit_dummy(self, event=None):
        self.root.quit()

    def show_menu(self, event):
        self.menu.tk_popup(event.x_root, event.y_root)

    def toggle_always_on_top(self, event=None):
        if event:
            self.always_on_top.set(not self.always_on_top.get())

        self.update_always_on_top()

    def update_always_on_top(self):
        self.root.attributes("-topmost", self.always_on_top.get())
        self.root.title(WINDOW_TITLE_PINNED if self.always_on_top.get() else WINDOW_TITLE)

    def toggle_use_black_bg(self, event=None):
        if event:
            self.use_black_bg.set(not self.use_black_bg.get())
        
        if self.use_black_bg.get():
            self.canvas.config(bg="black")
        else:
            self.canvas.config(bg="white")
        
        self.update_canvas()

    def toggle_keep_aspect_ratio(self, event=None):
        if event:
            self.keep_aspect_ratio.set(not self.keep_aspect_ratio.get())
        
        self.resize_image()

    def resize_window_to_image(self, event=None):
        if self.keep_aspect_ratio.get() and self.image_opened():
            new_size = self.get_image_size()
            self.root.geometry(f"{new_size[0]}x{new_size[1]}")
        
        self.update_canvas()

    def open_image_command(self, event=None):
        file_path = filedialog.askopenfilename(
            title="Select an Image",
            filetypes=[("Image Files", "*.png;*.jpg;*.jpeg;*.gif;*.bmp;*.tiff")]
        )
        # in this case, we just ignore it, because the user couldve clicked "Cancel"
        if file_path:
            if os.path.exists(file_path):
                self.image_path = file_path
                self.open_image()
            else:
                print(f"[ERROR]: Image path not found '{image_path}', provided in the file dialog! Continuing with imageless mode.")
    
    # checkbuttons return an event, but commands or keybinds dont
    def close_image(self, event=None):
        if self.image_opened():
            self.image_path = None
            self.resize_image()

    def toggle_use_antialiasing(self, event=None):
        if event:
            self.use_antialiasing.set(not self.use_antialiasing.get())
        
        self.resize_image()

    # IMAGE HANDLING
    def open_image(self):
        if not self.image_opened():
            self.resize_image()
            return

        self.img = Image.open(self.image_path)
        self.img_original = self.img.copy() 
        self.photo = ImageTk.PhotoImage(self.img)

        self.resize_image(None, True)
        #self.resize_window_to_image()

    def resize_image(self, event=None, resize_window=False):
        if not self.image_opened():
            self.update_canvas()
            return
        
        resammpling = Image.Resampling.LANCZOS if self.use_antialiasing.get() else Image.Resampling.NEAREST
        new_size = self.get_image_size()
        resized_img = self.img_original.resize(new_size, resammpling) #Image.Resampling.LANCZOS
        self.photo = ImageTk.PhotoImage(resized_img)

        if self.keep_aspect_ratio.get() and self.image_opened() and resize_window:
            self.root.geometry(f"{new_size[0]}x{new_size[1]}")

        self.update_canvas()

    def update_canvas(self):
        self.canvas.delete("all")
        actual_size = self.get_window_size()

        # apparently on initalization of tkinter window, winfo_width and height return 0 (or 1)
        if self.image_opened():
            self.canvas.create_image(
                actual_size[0] // 2, actual_size[1] // 2,
                anchor=tk.CENTER, image=self.photo
            )
        else:
            # should right click be "Right Click" or "RMB"? hmm...
            self.canvas.create_text(
                actual_size[0] // 2, actual_size[1] // 2,
                text=f"plainIMG v{VERSION}\nOpen Menu  [RMB]\nOpen Image [O]\nQuit       [Q]",
                #text=f"plainIMG\nOpen Menu  [RMB]\nOpen Image [O]\nQuit       [Q]",
                font=main_font,
                fill=("white" if self.use_black_bg.get() else "black"),
                #justify=tk.CENTER
            )

    def get_window_size(self):
        actual_size = (self.canvas.winfo_width(), self.canvas.winfo_height())
        if (actual_size[0] == 0 or actual_size[1] == 0) or (actual_size[0] == 1 or actual_size[1] == 1):
            actual_size = DEFAULT_SIZE
        return actual_size
        
    def get_image_size(self):
        win_w, win_h = self.get_window_size()
        #win_w, win_h = self.root.winfo_width(), self.root.winfo_height()
        img_w, img_h = self.img_original.size

        if self.keep_aspect_ratio.get():
            scale = min(win_w / img_w, win_h / img_h)
            new_size = (int(img_w * scale), int(img_h * scale))
        else:
            new_size = (win_w, win_h)

        return new_size

if __name__ == "__main__":
    root = tk.Tk()
    root.title(WINDOW_TITLE)
    root.geometry(f"{DEFAULT_SIZE[0]}x{DEFAULT_SIZE[1]}")
    root.minsize(MIN_SIZE[0], MIN_SIZE[1])
    root.configure(bg=BG_COLOR_INIT)
    root.iconbitmap('icon.ico')

    # imageless is when the application starts without an image provided at the start, which causes the starting text to display
    image_path = None
    if len(sys.argv) > 1:
        image_path = sys.argv[1]
        if not os.path.exists(image_path):
            print(f"[ERROR]: Image path not found '{image_path}', provided in the first argument! Starting with imageless mode.")
            image_path = None


    # FOR DEBUG WHILE DEVELOPING
    #image_path = "image.png"

    viewer = ImageViewer(root, image_path)

    try:
        root.mainloop()
    except KeyboardInterrupt:
        pass
