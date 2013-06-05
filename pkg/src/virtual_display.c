static void
virtual_crtc_dpms(xf86CrtcPtr intel_crtc, int mode)
{

}

static Bool
virtual_crtc_set_mode_major(xf86CrtcPtr crtc, DisplayModePtr mode,
			  Rotation rotation, int x, int y)
{
	return TRUE;
}

static void
virtual_crtc_set_cursor_colors(xf86CrtcPtr crtc, int bg, int fg)
{

}

static void
virtual_crtc_set_cursor_position (xf86CrtcPtr crtc, int x, int y)
{
}

static void
virtual_crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 *image)
{
}

static void
virtual_crtc_hide_cursor(xf86CrtcPtr crtc)
{
}

static void
virtual_crtc_show_cursor(xf86CrtcPtr crtc)
{
}

static void *
virtual_crtc_shadow_allocate(xf86CrtcPtr crtc, int width, int height)
{
	return NULL;
}

static PixmapPtr
virtual_crtc_shadow_create(xf86CrtcPtr crtc, void *data, int width, int height)
{
	return NULL;
}

static void
virtual_crtc_shadow_destroy(xf86CrtcPtr crtc, PixmapPtr rotate_pixmap, void *data)
{
}

static void
virtual_crtc_gamma_set(xf86CrtcPtr crtc,
		       CARD16 *red, CARD16 *green, CARD16 *blue, int size)
{
}

static void
virtual_crtc_destroy(xf86CrtcPtr crtc)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;

	list_del(&intel_crtc->link);
	free(intel_crtc);

	crtc->driver_private = NULL;
}

static const xf86CrtcFuncsRec virtual_crtc_funcs = {
	.dpms = virtual_crtc_dpms,
	.set_mode_major = virtual_crtc_set_mode_major,
	.set_cursor_colors = virtual_crtc_set_cursor_colors,
	.set_cursor_position = virtual_crtc_set_cursor_position,
	.show_cursor = virtual_crtc_show_cursor,
	.hide_cursor = virtual_crtc_hide_cursor,
	.load_cursor_argb = virtual_crtc_load_cursor_argb,
	.shadow_create = virtual_crtc_shadow_create,
	.shadow_allocate = virtual_crtc_shadow_allocate,
	.shadow_destroy = virtual_crtc_shadow_destroy,
	.gamma_set = virtual_crtc_gamma_set,
	.destroy = virtual_crtc_destroy,
};

static void
virtual_crtc_init(ScrnInfoPtr scrn, struct intel_mode *mode)
{
	xf86CrtcPtr crtc;
	struct intel_crtc *intel_crtc;

	intel_crtc = calloc(sizeof(struct intel_crtc), 1);
	if (intel_crtc == NULL)
		return;

	crtc = xf86CrtcCreate(scrn, &virtual_crtc_funcs);
	if (crtc == NULL) {
		free(intel_crtc);
		return;
	}

	intel_crtc->mode = mode;
	intel_crtc->crtc = crtc;
	crtc->driver_private = intel_crtc;
	list_add(&intel_crtc->link, &mode->crtcs);
}

static xf86OutputStatus
virtual_output_detect(xf86OutputPtr output)
{
	// return XF86OutputStatusConnected;
	// return XF86OutputStatusDisconnected;
	return XF86OutputStatusUnknown;
}

static Bool
virtual_output_mode_valid(xf86OutputPtr output, DisplayModePtr pModes)
{
	return MODE_OK;
}

static DisplayModePtr
virtual_output_get_modes(xf86OutputPtr output)
{
	DisplayModePtr i, m, p = NULL;
	int max_x = 1920, max_y = 1200;
	float max_vrefresh = 60.0;

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,6,99,0,0)
	m = xf86GetDefaultModes();
#else
	m = xf86GetDefaultModes(0,0);
#endif

	xf86ValidateModesSize(output->scrn, m, max_x, max_y, 0);

	for (i = m; i; i = i->next) {
		if (xf86ModeVRefresh(i) > max_vrefresh)
			i->status = MODE_VSYNC;
		if (p && i->HDisplay >= p->HDisplay &&
		    i->VDisplay >= p->VDisplay &&
		    xf86ModeVRefresh(i) >= xf86ModeVRefresh(p))
			i->status = MODE_VSYNC;
	}

	xf86PruneInvalidModes(output->scrn, &m, FALSE);

	return m;
}

static void
virtual_output_destroy(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;

	list_del(&intel_output->link);
	free(intel_output);

	output->driver_private = NULL;
}

static void
virtual_output_dpms(xf86OutputPtr output, int dpms)
{
}

static void
virtual_output_create_resources(xf86OutputPtr output)
{
}

static Bool
virtual_output_set_property(xf86OutputPtr output, Atom property,
			    RRPropertyValuePtr value)
{
	return TRUE;
}

static Bool
virtual_output_get_property(xf86OutputPtr output, Atom property)
{
	return FALSE;
}

static const xf86OutputFuncsRec virtual_output_funcs = {
	.create_resources = virtual_output_create_resources,
#ifdef RANDR_12_INTERFACE
	.set_property = virtual_output_set_property,
	.get_property = virtual_output_get_property,
#endif
	.dpms = virtual_output_dpms,
	.detect = virtual_output_detect,
	.mode_valid = virtual_output_mode_valid,

	.get_modes = virtual_output_get_modes,
	.destroy = virtual_output_destroy
};

static void
virtual_output_init(ScrnInfoPtr scrn, struct intel_mode *mode, int index)
{
	xf86OutputPtr output;
	struct intel_output *intel_output;
	char name[32];
	sprintf( name, "VIRTUAL%d", index + 1 );

	output = xf86OutputCreate (scrn, &virtual_output_funcs, name);
	if (!output) {
		return;
	}

	intel_output = calloc(sizeof(struct intel_output), 1);
	if (!intel_output) {
		xf86OutputDestroy(output);
		return;
	}

	output->subpixel_order = SubPixelHorizontalRGB;
	output->possible_crtcs = (1 << (mode->mode_res->count_crtcs + index));
	output->driver_private = intel_output;
	intel_output->output = output;
	intel_output->mode = mode;

	list_add(&intel_output->link, &mode->outputs);
}

static Bool
is_virtual(struct intel_mode *mode, int i)
{
    return i >= mode->mode_res->count_crtcs;
}
