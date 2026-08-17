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

use std::ffi::c_void;
use std::ptr;

#[repr(C)]
pub enum AuraCommandType {
    DrawRect = 0,
    DrawText = 1,
    DrawImage = 2,
    DrawVideoFrame = 3,
}

#[repr(C)]
#[derive(Clone, Copy, Debug, Default)]
pub struct AuraColor {
    pub r: u8,
    pub g: u8,
    pub b: u8,
    pub a: u8,
}

#[repr(C)]
#[derive(Clone, Copy, Debug, Default)]
pub struct AuraRect {
    pub x: f32,
    pub y: f32,
    pub width: f32,
    pub height: f32,
}

#[repr(C)]
pub struct AuraDisplayCommand {
    pub cmd_type: AuraCommandType,
    pub rect: AuraRect,
    pub color: AuraColor,
    pub data_ptr: *const c_void,
}

#[repr(C)]
pub struct AuraDisplayList {
    pub commands: *const AuraDisplayCommand,
    pub count: usize,
}

#[unsafe(no_mangle)]
pub extern "C" fn aura_rust_build_display_list_commands() -> *mut AuraDisplayList {
    let mut cmds = vec![AuraDisplayCommand {
        cmd_type: AuraCommandType::DrawRect,
        rect: AuraRect { x: 0.0, y: 0.0, width: 800.0, height: 600.0 },
        color: AuraColor { r: 255, g: 255, b: 255, a: 255 },
        data_ptr: ptr::null(),
    }];

    let commands_ptr = cmds.as_mut_ptr();
    let count = cmds.len();
    std::mem::forget(cmds);

    let list = Box::new(AuraDisplayList {
        commands: commands_ptr,
        count,
    });

    Box::into_raw(list)
}

#[unsafe(no_mangle)]
pub extern "C" fn aura_rust_free_display_list_commands(list_ptr: *mut AuraDisplayList) {
    if list_ptr.is_null() {
        return;
    }

    unsafe {
        let list = Box::from_raw(list_ptr);
        let _ = Vec::from_raw_parts(list.commands as *mut AuraDisplayCommand, list.count, list.count);
    }
}
