/*
 * Aura Core
 * Copyright (C) 2026 Aura-core-pacur
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

pub mod display_list;

use std::ffi::{CStr, CString};
use std::os::raw::c_char;

#[repr(C)]
#[derive(Clone, Copy)]
pub struct AuraRect {
    pub x: f32,
    pub y: f32,
    pub width: f32,
    pub height: f32,
    pub color: u32,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct AuraText {
    pub x: f32,
    pub y: f32,
    pub color: u32,
    pub size: f32,
    pub text_ptr: *const c_char,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct AuraImage {
    pub x: f32,
    pub y: f32,
    pub width: f32,
    pub height: f32,
    pub image_ptr: *const c_char,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct AuraBorder {
    pub x: f32,
    pub y: f32,
    pub width: f32,
    pub height: f32,
    pub stroke: f32,
    pub color: u32,
}

#[repr(C)]
pub struct AuraDisplayList {
    pub rect_count: u32,
    pub text_count: u32,
    pub image_count: u32,
    pub border_count: u32,
    pub rects: *mut AuraRect,
    pub texts: *mut AuraText,
    pub images: *mut AuraImage,
    pub borders: *mut AuraBorder,
}

#[unsafe(no_mangle)]
pub extern "C" fn aura_rust_parse_html(input: *const c_char) -> *mut c_char {
    let text = if input.is_null() {
        String::new()
    } else {
        unsafe { CStr::from_ptr(input) }.to_string_lossy().into_owned()
    };

    let output = format!("[AuraRust] Parsed HTML: {}", text);
    let c_string = CString::new(output).unwrap_or_else(|_| CString::new("[AuraRust] Parsed HTML").unwrap());
    c_string.into_raw()
}

#[unsafe(no_mangle)]
pub extern "C" fn aura_rust_parse_css(input: *const c_char) -> *mut c_char {
    let text = if input.is_null() {
        String::new()
    } else {
        unsafe { CStr::from_ptr(input) }.to_string_lossy().into_owned()
    };

    let output = format!("[AuraRust] Parsed CSS: {}", text);
    let c_string = CString::new(output).unwrap_or_else(|_| CString::new("[AuraRust] Parsed CSS").unwrap());
    c_string.into_raw()
}

#[unsafe(no_mangle)]
pub extern "C" fn aura_rust_build_display_list(dom_ptr: *const c_char) -> *const AuraDisplayList {
    let _ = dom_ptr;

    let rect = AuraRect { x: 0.0, y: 0.0, width: 100.0, height: 40.0, color: 0xFF3366FF };
    let text = AuraText { x: 10.0, y: 20.0, color: 0xFFFFFFFF, size: 16.0, text_ptr: b"Aura Core\0".as_ptr() as *const c_char };
    let border = AuraBorder { x: 0.0, y: 0.0, width: 120.0, height: 50.0, stroke: 2.0, color: 0xFF000000 };

    let display = Box::new(AuraDisplayList {
        rect_count: 1,
        text_count: 1,
        image_count: 0,
        border_count: 1,
        rects: Box::into_raw(Box::new(rect)),
        texts: Box::into_raw(Box::new(text)),
        images: std::ptr::null_mut(),
        borders: Box::into_raw(Box::new(border)),
    });

    Box::into_raw(display)
}

#[unsafe(no_mangle)]
pub extern "C" fn aura_rust_native_build_display_list(dom_ptr: *const c_char) -> *const AuraDisplayList {
    aura_rust_build_display_list(dom_ptr)
}

#[unsafe(no_mangle)]
pub extern "C" fn aura_rust_free_display_list(display: *mut AuraDisplayList) {
    if display.is_null() {
        return;
    }

    unsafe {
        let list = Box::from_raw(display);
        if !list.rects.is_null() { let _ = Box::from_raw(list.rects); }
        if !list.texts.is_null() { let _ = Box::from_raw(list.texts); }
        if !list.images.is_null() { let _ = Box::from_raw(list.images); }
        if !list.borders.is_null() { let _ = Box::from_raw(list.borders); }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn aura_rust_native_free_display_list(display: *mut AuraDisplayList) {
    aura_rust_free_display_list(display)
}

#[cfg(test)]
mod tests {
    #[test]
    fn smoke() {
        let _ = 1 + 1;
    }
}
